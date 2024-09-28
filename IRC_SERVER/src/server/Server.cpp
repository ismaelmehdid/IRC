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

bool    Server::isNickNameTaken(const std::string &nickName)
{
    return (std::find(_nicknames.begin(), _nicknames.end(), nickName) != _nicknames.end());
}

/**
 * @brief The main server loop for handling connections and events.
 * 
 * This function continuously polls the file descriptors in the _fds array for events.
 * If poll fails, it throws a PollException. For each event, it delegates to handlePollEvent().
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

        for (size_t i = 0; i < _fds.size() && _poll_count > 0; )
        {
            if (_fds[i].revents != 0)
            {
                handlePollEvent(i);
                _poll_count--; // Decrement the count of events to handle
                // Only increment i if no element was removed
                if (i < _fds.size() && _fds[i].revents == 0)
                    i++;
            }
            else
                i++;
        }
    }
}

/**
 * @brief Starts the server and sets it up for accepting connections.
 * 
 * This function creates the server socket, binds it to a port, and starts listening for connections.
 * It sets up the server file descriptor in the pollfd array and enters the ServerLoop() to begin processing connections.
 * 
 * @param argv Command-line arguments. argv[1] should contain the port number to bind to.
 * @throws ServerCreationException if socket creation fails.
 * @throws ServerBindException if binding the socket fails.
 * @throws ServerListenException if the server fails to start listening for connections.
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