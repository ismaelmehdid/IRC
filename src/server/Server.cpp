#include "../../include/server/Server.hpp"

Server::Server(const std::string &password)
    : _nbr_clients(0), _password(password), _socket(), _clients(), _channels() {}

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

void    Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
}

void    Server::removeClient(int fd)
{
    delete (this->_clients[fd]);
    this->_clients.erase(fd);
    this->_nbr_clients--;
}

const std::string Server::get_password() const
{
    return (this->_password);
}

void Server::ServerLoop()
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

void    Server::RunServer(char **argv)
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

    ServerLoop();
}