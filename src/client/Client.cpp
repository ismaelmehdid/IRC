#include "../../include/irc.hpp"
#include "../../include/server/Client.hpp"

Client::Client() : _fd(-1), _role(new RegularRole()) {}

Client::Client(int fd, IRole* role) : _fd(fd), _role(role) {}

Client::~Client()
{
    std::cout << "Client destroyed." << std::endl;
    delete _role;
    close(_fd);
}

int Client::get_fd() const
{
    return _fd;
}

void Client::executeKick()
{
    this->_role->kick();
}

void Client::executeInvite()
{
    this->_role->invite();
}

void Client::executeTopic()
{
    this->_role->topic();
}

void Client::executeMode(char arg)
{
    this->_role->changeMode(arg);
}

void Client::setRole(IRole* newRole)
{
    delete (this->_role);
    this->_role = newRole;
}