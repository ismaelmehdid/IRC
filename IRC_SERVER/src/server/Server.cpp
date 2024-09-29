#include "../../include/server/Server.hpp"

Server::Server(const std::string &password)
    :   _nbr_clients(0),
        _password(password),
        _clients(),
        _channels(),
        _fds(),
        _server_pollfd(),
        _poll_count(0),
        _socket(),
        _nicknames() { initializeCommandMap(); }

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        delete (it->second);
    }

    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        delete (it->second);
    }
}

Server::Server(const Server &server) // shallow copy
    :   _nbr_clients(server._nbr_clients),
        _password(server._password),
        _clients(server._clients),
        _channels(server._channels),
        _fds(server._fds),
        _server_pollfd(server._server_pollfd),
        _poll_count(server._poll_count),
        _socket(),
        _nicknames(server._nicknames) {}

Server &Server::operator=(const Server &server) // shallow copy
{
    if (this != &server)
    {
        _nbr_clients = server._nbr_clients;
        _password = server._password;
        _clients = server._clients;
        _channels = server._channels;
        _fds = server._fds;
        _server_pollfd = server._server_pollfd;
        _poll_count = server._poll_count;
        _nicknames = server._nicknames;
        // _socket default constructor called
    }
    return *this;
}

/**
 * @brief Initializes the command map with IRC command strings as keys and 
 *        corresponding member function pointers as values.
 * 
 * This function sets up the _commandMap by associating various IRC command 
 * strings (e.g., "KICK", "INVITE", "TOPIC") with their respective handler 
 * member functions (e.g., &Server::kick, &Server::invite, &Server::topic).
 * This allows the server to dynamically call the appropriate function 
 * based on the received command.
 */
void    Server::initializeCommandMap()
{
    _commandMap["KICK"]     = &Server::kick;
    _commandMap["INVITE"]   = &Server::invite;
    _commandMap["TOPIC"]    = &Server::topic;
    _commandMap["MODE"]     = &Server::mode;
    _commandMap["JOIN"]     = &Server::join;
    _commandMap["PART"]     = &Server::part;
    _commandMap["PRIVMSG"]  = &Server::privMsg;
    _commandMap["PING"]     = &Server::ping;
    _commandMap["CAP"]      = &Server::cap;
    _commandMap["PASS"]     = &Server::pass;
    _commandMap["NICK"]     = &Server::nick;
    _commandMap["USER"]     = &Server::user;
    _commandMap["QUIT"]     = &Server::quit;
}

/**
 * @brief Checks if a given nickname is already taken.
 * 
 * This function searches through the list of nicknames to determine if the 
 * specified nickname is already in use.
 * 
 * @param nickName The nickname to check for availability.
 * @return true if the nickname is already taken, false otherwise.
 */
bool    Server::isNickNameTaken(const std::string &nickName)
{
    return (std::find(_nicknames.begin(), _nicknames.end(), nickName) != _nicknames.end());
}

/**
 * @brief Main server loop that continuously polls for events and handles them.
 * 
 * This function runs an infinite loop that uses the poll system call to monitor
 * multiple file descriptors for events. When an event is detected, it is handled
 * by the handlePollEvent function. If poll returns an error, a PollException is thrown.
 * 
 * @throws PollException if the poll system call fails.
 */
void    Server::serverLoop()
{
    while (true)
    {
        this->_poll_count = poll(_fds.data(), _fds.size(), -1);
        
        if (this->_poll_count == -1)
        {
            throw PollException();
        }

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents != 0)
            {
                handlePollEvent(i);
            }
        }
    }
}

/**
 * @brief Runs the server with the specified arguments.
 *
 * This function initializes the server by creating a socket, binding it to the specified port,
 * and setting it to listen for incoming connections. It also sets up the polling file descriptor
 * and starts the server loop to handle incoming connections.
 *
 * @param argv Command-line arguments, where argv[1] is expected to be the port number.
 *
 * @throws ServerCreationException If the socket creation fails.
 * @throws ServerBindException If the socket binding fails.
 * @throws ServerListenException If the socket fails to listen for connections.
 */
void    Server::runServer(char **argv)
{
    if (!this->_socket.create())
        throw ServerCreationException();
    if (!this->_socket.bind(std::strtol(argv[1], NULL, 10)))
        throw ServerBindException();
    if (!this->_socket.listen())
        throw ServerListenException();

    this->_server_pollfd.fd = this->_socket.get_fd();
    this->_server_pollfd.events = POLLIN;
    this->_fds.push_back(this->_server_pollfd);

    std::cout << "Waiting for connections on port "
              << argv[1] << "..." << std::endl;

    serverLoop();
}