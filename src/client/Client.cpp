#include "../../include/server/Server.hpp"

Client::Client(int fd, char	*host)
    :   _fd(fd),
        _hostMask(host),
        _nickName(),
        _userName(),
        _fullName(),
        _has_set_password(false) {}

Client::Client (const Client& other)
    :   _fd(other.get_fd()),
        _nickName(other._nickName),
        _userName(other._userName),
        _fullName(other._fullName),
        _has_set_password(other._has_set_password) {}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        close (this->_fd);
        this->_fd = other.get_fd();
        this->_nickName = other._nickName;
        this->_userName = other._userName;
        this->_fullName = other._fullName;
        this->_has_set_password = other._has_set_password;
    }
    return (*this);
}

Client::~Client()
{
    close (this->_fd);
}

//----------Getters
std::string Client::getNickName() const
{
    return (this->_nickName);
}

std::string Client::getUserName() const
{
    return (this->_userName);
}

std::string Client::getFullName() const
{
    return (this->_fullName);
}

std::string Client::getHostMask() const
{
    return (this->_hostMask);
}

std::string Client::getPrefix() const
{
    return (":" + this->_nickName + "!" + this->_userName + "@" + this->_hostMask);
}

int Client::get_fd() const
{
    return (this->_fd);
}

bool    Client::is_authenticated()
{
    return (_has_set_password && !_nickName.empty() && !_userName.empty() && !_fullName.empty());
}

//------Setters
void    Client::setNickName(const std::string &nickName)
{
   this->_nickName = nickName;
}

void    Client::setUserName(const std::string &userName)
{
    this->_userName = userName;
}

void    Client::setFullName(const std::string &fullName)
{
    this->_fullName = fullName;
}
