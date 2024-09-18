#include "../../include/channel/Channel.hpp"
#include "../../include/irc.hpp"
#include "../../include/server/Server.hpp"

Channel::Channel(const std::string& name)
    : _name(name), 
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


void    Channel::broadcastMessage(const std::string& message)
{
    if (message.empty())
    {
        std::cerr << "Attempted to broadcast an empty message in channel " << _name << std::endl;
        return ;
    }
    
    for (std::set<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << "Sending message: " << message << " to client " << (*it)->get_fd() << std::endl;
        if (!global_ircserv->socketSend((*it)->get_fd(), message))
        {
            std::cerr << "Failed to send message to client " <<(*it)->get_fd() << std::endl;
        }
    }
}

//------Client Management
void    Channel::addClient(Client* client)
{
    if (this->_clients.find(client) == this->_clients.end())
    {
        this->_clients.insert(client);
    }
    else
    {
        std::cerr << "Client " << client->get_fd() << " is already in channel " << _name << std::endl;
    }
}

void    Channel::removeClient(Client* client)
{
    std::set<Client *>::iterator it = _clients.find(client);

    if (it != this->_clients.end())
    {
        this->_clients.erase(it);
        std::cout << "Client " << client->get_fd() << " removed from channel " << _name << std::endl;
    }
    else
    {
        std::cerr << "Client " << client->get_fd() << " not found in channel " << _name << std::endl;
    }
}

void    Channel::addOperator(Client* client)
{
    if (isMember(client))
        this->_operators.insert(client);
}

void    Channel::removeOperator(Client* client)
{
    if (isMember(client))
        this->_operators.erase(client);
}

//------Getters
int Channel::getUserLimit() const
{
    return (this->_userLimit);
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

bool    Channel::isMember(Client* client) const
{
    return (this->_clients.find(client) != this->_clients.end());
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