#include "../../include/server/Server.hpp"

Client::Client(int fd, char	*host)
    :   _fd(fd),
        _hostMask(host),
        _nickName(),
        _userName(),
        _fullName(),
        _has_set_password(false) { initializeCommandMap(); }

Client::Client (const Client& other)
    :   _fd(other.get_fd()),
        _nickName(other._nickName),
        _userName(other._userName),
        _fullName(other._fullName),
        _has_set_password(other._has_set_password) { initializeCommandMap(); }

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
    std::cout << GREEN <<"Client destroyed." << RESET << std::endl;
    close (this->_fd);
}

void    Client::initializeCommandMap()
{
    _commandMap["KICK"]    = &Client::executeKick;
    _commandMap["INVITE"]  = &Client::executeInvite;
    _commandMap["TOPIC"]   = &Client::executeTopic;
    _commandMap["MODE"]    = &Client::executeMode;
    _commandMap["CAP"]     = &Client::executeCap;
    _commandMap["PASS"]    = &Client::executePass;
    _commandMap["NICK"]    = &Client::executeNick;
    _commandMap["USER"]    = &Client::executeUser;
    _commandMap["JOIN"]    = &Client::executeJoin;
    _commandMap["PART"]    = &Client::executePart;
    _commandMap["PRIVMSG"] = &Client::executePrivMsg;
    _commandMap["PING"]    = &Client::executePing;
    _commandMap["QUIT"]    = &Client::executeQuit;
}

bool    Client::is_authenticated()
{
    return (_has_set_password && !_nickName.empty() && !_userName.empty() && !_fullName.empty());
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
