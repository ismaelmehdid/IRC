#include "../../include/server/Server.hpp"

void    Server::handleClientMessage(size_t i)
{
    std::string message = _socket.receive(_fds[i].fd);
    if (message.empty())
    {
        handleClientDisconnection(i);
    }
    else
    {
        std::cout << "Message received from client: " << message << std::endl;
        _socket.send(_fds[i].fd, "Message received\n");
    }
}

void    Server::handleNewConnection()
{
    int client_fd = _socket.accept();
    if (client_fd != -1)
    {
        pollfd client_pollfd;
        client_pollfd.fd = client_fd;
        client_pollfd.events = POLLIN;
        _fds.push_back(client_pollfd);
        
        Client* newClient = perform_handshake(client_fd);
        if (newClient)
        {
            addClient(newClient);
            std::cout << "Client connected!" << std::endl;
            std::string welcome_msg = ":server 001 client :Welcome to the IRC server!\r\n";
            _socket.send(client_fd, welcome_msg);
        }
    }
}

void    Server::handleClientDisconnection(size_t i)
{
    std::cerr << "Client error or hangup on fd " << _fds[i].fd << std::endl;
    close(_fds[i].fd);
    removeClient(_fds[i].fd);
    _fds.erase(_fds.begin() + i);
}

void    Server::handlePollEvent(size_t i)
{
    if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
    {
        handleClientDisconnection(i);
    }
    else if (_fds[i].revents & POLLIN)
    {
        if (_fds[i].fd == _socket.get_fd())
        {
            handleNewConnection();
        }
        else
        {
            handleClientMessage(i);
        }
    }
}