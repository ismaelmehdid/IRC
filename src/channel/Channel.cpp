#include "../../include/channel/Channel.hpp"
#include "../../include/irc.hpp"
#include "../../include/server/Server.hpp"

Channel::Channel(const std::string& name)
    : _name(name), _topic(""), _clients() {}

Channel::~Channel()
{
    for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        delete (it->second);
    }
    this->_clients.clear();
}

void    Channel::addClient(Client* client)
{
    if (this->_clients.find(client->get_fd()) == this->_clients.end())
    {
        this->_clients[client->get_fd()] = client;
    }
    else
    {
        std::cerr << "Client " << client->get_fd() << " is already in channel " << _name << std::endl;
    }
}


void    Channel::removeClient(Client* client)
{
    std::map<int, Client*>::iterator    it = _clients.find(client->get_fd());

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

void    Channel::broadcastMessage(const std::string& message)
{
    if (message.empty())
    {
        std::cerr << "Attempted to broadcast an empty message in channel " << _name << std::endl;
        return ;
    }
    
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << "Sending message: " << message << " to client " << it->first << std::endl;
        if (!global_ircserv->_socket.send(it->second->get_fd(), message))
        {
            std::cerr << "Failed to send message to client " << it->first << std::endl;
        }
    }
}

void    Channel::setTopic(const std::string& topic)
{
    this->_topic = topic;
    std::cout << "Topic for channel " << _name << " set to: " << _topic << std::endl;
}

const std::string&  Channel::getName() const
{
    return (this->_name);
}

const std::string&  Channel::getTopic() const
{
    return (this->_topic);
}

const std::map<int, Client*>&   Channel::getClients() const
{
    return (this->_clients);
}

bool    Channel::isMember(Client* client) const
{
    return (this->_clients.find(client->get_fd()) != this->_clients.end());
}