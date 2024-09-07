#include "../../include/irc.hpp"
#include "../../include/server/Client.hpp"

Client::Client() : _fd(-1) {}

Client::Client(int fd) : _fd(fd) {}

Client::~Client()
{
    std::cout << "Client destroyed." << std::endl;
    close(_fd);
}

int Client::get_fd() const
{
    return _fd;
}