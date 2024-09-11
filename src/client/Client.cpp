#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"

Client::Client() : _nickName("noName"), _userName("noUserName"), _fullName("noFullName"), _fd(-1), _role(new RegularRole()) {}

Client::Client(const std::string& nickName, const std::string& userName, const std::string& fullName, int fd, IRole* role)
                : _nickName(nickName), _userName(userName), _fullName(fullName), _fd(fd), _role(role) {}

Client::Client (const Client& other)
                : _nickName(other._nickName), _userName(other._userName), _fullName(other._fullName), _fd(other.get_fd()),
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
        this->_userName = other._userName;
        this->_fullName = other._fullName;
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