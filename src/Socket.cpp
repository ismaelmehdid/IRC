#include "../include/Socket.hpp"

Socket::Socket() : _sock_fd(-1), _backlog(5) {}

Socket::~Socket()
{
    close();
}

bool Socket::create()
{
    this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_sock_fd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return (false);
    }
    return (true);
}

bool    Socket::bind(int port)
{
    sockaddr_in addr;

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(this->_sock_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Bind failed" << std::endl;
        return (false);
    }
    return (true);
}

bool    Socket::listen()
{
    if (::listen(this->_sock_fd, this->_backlog) == -1)
    {
        std::cerr << "Listen failed" << std::endl;
        return (false);
    }
    return (true);
}

int Socket::accept()
{
    sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;
    
    client_fd = ::accept(this->_sock_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
        std::cerr << "Accept failed" << std::endl;

    return (client_fd);
}

bool    Socket::send(int client_fd, const std::string &message)
{
    if (::send(client_fd, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Send failed" << std::endl;
        return (false);
    }
    return (true);
}

std::string Socket::receive(int client_fd)
{
    char    buffer[1024];
    int     bytes_received;

    std::memset(buffer, 0, sizeof(buffer));
    bytes_received = ::recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received == -1)
    {
        std::cerr << "Receive failed" << std::endl;
        return ("");
    }

    return (std::string(buffer));
}

void    Socket::close()
{
    if (this->_sock_fd != -1)
    {
        ::close(this->_sock_fd);
        this->_sock_fd = -1;
    }
}

void    Socket::close_client(int client_fd)
{
    if (client_fd != -1)
    {
        ::close(client_fd);
    }
}