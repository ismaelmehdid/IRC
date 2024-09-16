#include "../../include/server/Server.hpp"

Server::Server(const std::string &password)
    :   _nbr_clients(0),
        _password(password),
        _clients(),
        _channels(),
        _fds(),
        _server_pollfd(),
        _poll_count(0),
        _socket() {}

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
void    Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
}

void Server::removeClient(Client* user, std::string reason)
{
    std::list<std::string>  empty_channels;
    int                     fd = user->get_fd();

    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        Channel*            channel = it->second;
        
        if (channel->isMember(user))
        {
            channel->removeClient(user);
            if (channel->getClients().empty())
                empty_channels.push_back(channel->getName());

            for (std::map<int, Client*>::const_iterator iter = channel->getClients().begin(); iter != channel->getClients().end(); ++iter)
            {
                this->_socket.send(iter->second->get_fd(), user->getPrefix() + " QUIT :" + reason);
            }
        }
    }

    for (std::list<std::string>::const_iterator it = empty_channels.begin(); it != empty_channels.end(); ++it)
    {
        delete (this->_channels[*it]);
        this->_channels.erase(*it);
    }

    for (int i = 0; i < this->_poll_count; ++i)
    {
        if (this->_fds[i].fd == fd)
        {
            pollRemove(i);
            break ;
        }
    }

    delete (this->_clients[fd]);
    this->_clients.erase(fd);
    this->_nbr_clients--;
}


void Server::pollRemove(int index)
{
	close(this->_fds[index].fd);
	this->_fds[index].fd = this->_fds[this->_poll_count - 1].fd;
	this->_fds[index].events = POLLIN;
	this->_fds[this->_poll_count - 1].fd = -1;

	--this->_poll_count;
}

const   std::string Server::get_password() const
{
    return (this->_password);
}

bool    Server::isNickNameTaken(const std::string &nickName)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNickName() == nickName)
        {
            return (true);
        }
    }
    return (false);
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
