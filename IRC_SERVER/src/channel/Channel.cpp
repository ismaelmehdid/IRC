#include "../../include/channel/Channel.hpp"
#include "../../include/irc.hpp"
#include "../../include/server/Server.hpp"

Channel::Channel(const std::string& name)
    : _name(name),
      _nbr_users(0),
      _topic(""), 
      _clients(),
      _operators(),
      _invited(),
      _inviteOnly(false), 
      _topicLocked(false), 
      _password(""),
      _userLimit(-1) {}

Channel::~Channel()
{
    _clients.clear();
    _operators.clear();
    _invited.clear();
}

Channel::Channel(const Channel &channel) // shallow copy
    :   _name(channel._name),
        _nbr_users(channel._nbr_users),
        _topic(channel._topic),
        _clients(channel._clients),
        _operators(channel._operators),
        _invited(channel._invited),
        _inviteOnly(channel._inviteOnly),
        _topicLocked(channel._topicLocked),
        _password(channel._password),
        _userLimit(channel._userLimit) {}

Channel &Channel::operator=(const Channel &channel) // shallow copy
{
    if (this != &channel) {
        _name = channel._name;
        _nbr_users = channel._nbr_users;
        _topic = channel._topic;
        _clients = channel._clients;
        _operators = channel._operators;
        _invited = channel._invited;
        _inviteOnly = channel._inviteOnly;
        _topicLocked = channel._topicLocked;
        _password = channel._password;
        _userLimit = channel._userLimit;
    }
    return *this;
}

//------Client Management
void    Channel::addClient(Client* user)
{
    if (isMember(user))
        return ;
    
    this->_clients.insert(user);
    this->_nbr_users++;
}

void    Channel::removeClient(Client* user)
{
    if (!isMember(user))
        return ;

    if (isOperator(user))
        this->_operators.erase(user);
    
    if (isInvited(user))
        this->_invited.erase(user);

    this->_clients.erase(user);
    this->_nbr_users--;
}

void Channel::addOperator(Client* client)
{
    if (!isMember(client))
        return ;

    if (isOperator(client))
        return ;

    this->_operators.insert(client);
}

void    Channel::removeOperator(Client* client)
{
    if (!isMember(client))
        return ;

    if (!isOperator(client))
        return;

    this->_operators.erase(client);
}

void    Channel::addInvited(Client* client)
{
    if (isMember(client))
        return ;
    
    if (isInvited(client))
        return ;

    this->_invited.insert(client);
}

void    Channel::removeInvited(Client* client)
{
    if (!isMember(client))
        return ;

    if (!isInvited(client))
        return ;

    this->_invited.erase(client);
}

//------Getters
int Channel::getUserLimit() const
{
    return (this->_userLimit);
}

int Channel::getNbrUsers() const
{
    return (this->_nbr_users);
}

const std::string&  Channel::getName() const
{
    return (this->_name);
}

const std::string&  Channel::getTopic() const
{
    return (this->_topic);
}

const std::set<Client *>&   Channel::getClients() const
{
    return (this->_clients);
}
const std::set<Client *>&  Channel::getOperators() const
{
    return (this->_operators);
}

bool    Channel::isMember(Client* client) const
{
    return (this->_clients.find(client) != this->_clients.end());
}

bool    Channel::isTopicLocked() const
{
    return (this->_topicLocked);
}

bool    Channel::isInviteOnly() const
{
    return (this->_inviteOnly);
}

bool    Channel::hasPassword() const
{
    if (this->_password.empty())
        return (false);
    else
        return (true);
}

bool    Channel::checkPassword(const std::string& key) const
{
    return (this->_password == key);
}

bool    Channel::isOperator(Client* client) const
{
    return (this->_operators.find(client) != _operators.end());
}

bool    Channel::isInvited(Client* client) const
{
    return (this->_invited.find(client) != _invited.end());
}

//------Setters
void    Channel::setTopicLocked(bool mode)
{
    this->_topicLocked = mode;
}

void    Channel::setInviteOnly(bool mode)
{
    this->_inviteOnly = mode;
}

void    Channel::setPassword(const std::string& key)
{
    this->_password = key;
}

void    Channel::setUserLimit(int limit)
{
    this->_userLimit = limit;
}

void    Channel::setTopic(const std::string& topic)
{
    this->_topic = topic;
}