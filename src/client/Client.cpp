#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"

Client::Client() : _nickName("noname"), _fd(-1), _role(new RegularRole()) {}

Client::Client(const std::string& nickName, int fd, IRole* role)
                : _nickName(nickName), _fd(fd), _role(role) {}

Client::Client (const Client& other)
                : _nickName(other._nickName), _fd(other.get_fd()),
                  _role(other._role) {}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        delete (this->_role);
        close (this->_fd);
        this->_fd = other.get_fd();
        this->_role = other._role;
        this->_nickName = other._nickName;
    }
    return (*this);
}

Client::~Client()
{
    std::cout << "Client destroyed." << std::endl;
    delete (this->_role);
    close (this->_fd);
}

int Client::get_fd() const
{
    return (this->_fd);
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
    this->_role->mode(arg);
}

void Client::setRole(IRole* newRole)
{
    delete (this->_role);
    this->_role = newRole;
}