#include "../../include/server/Server.hpp"

Server::Server(const std::string &password)
    : _nbr_clients(0), _password(password), _clients(), _channels(), _socket() {}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        delete (it->second);
    }

    for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
    {
        close (it->fd);
    }
}

/**
 * @brief Adds a new client to the server.
 * 
 * This function stores the client in the _clients map, 
 * using the client's file descriptor as the key, and increments the total number of clients.
 * 
 * @param client A pointer to the Client object to be added.
 */
void Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
}

/**
 * @brief Removes a client from the server.
 * 
 * This function deletes the Client object corresponding to the given file descriptor,
 * removes it from the _clients map, and decrements the total number of clients.
 * 
 * @param fd The file descriptor of the client to be removed.
 */
void Server::removeClient(int fd)
{
    delete (this->_clients[fd]);
    this->_clients.erase(fd);
    this->_nbr_clients--;
}

/**
 * @brief Retrieves the server's password.
 * 
 * This function returns the password required to connect to the server.
 * 
 * @return The server password as a const string.
 */
const std::string Server::get_password() const
{
    return (this->_password);
}

/**
 * @brief The main server loop for handling connections and events.
 * 
 * This function continuously polls the file descriptors in the _fds array for events.
 * If poll fails, it throws a PollException. For each event, it delegates to handlePollEvent().
 */
void Server::serverLoop()
{
    while (true)
    {
        _poll_count = poll(_fds.data(), _fds.size(), -1);

        if (_poll_count == -1)
            throw PollException();

        for (size_t i = 0; i < _fds.size(); i++)
        {
            handlePollEvent(i);
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
void Server::runServer(char **argv)
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

bool Server::isNickNameTaken(const std::string &nickName)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second->getNickName() == nickName) {
            return (true);
        }
    }
    return (false);
}
