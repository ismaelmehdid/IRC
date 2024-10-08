#include "../../include/server/Server.hpp"

Channel*    Server::createChannel(const std::string& channelName)
{
    if (_channels.find(channelName) == _channels.end())
    {
        Channel*    new_channel = new Channel(channelName);
        this->_channels[channelName] = new_channel;
        return  (new_channel);
    }
    return (NULL);
}

Channel*    Server::findChannel(const std::string& channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    
    if (it != _channels.end())
        return (it->second);
    
    return (NULL);
}

void    Server::removeChannel(Channel* channel)
{
    std::map<std::string, Channel*>::iterator it = this->_channels.find(channel->getName());
    
    if (it != this->_channels.end())
    {
        delete (it->second);
        this->_channels.erase(it);
    }
}

void    Server::broadcastMessage(const std::string& msg, Channel* channel)
{
    if (msg.empty())
        return ;
    
    const std::set<Client*>& clients = channel->getClients();

    for (std::set<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        this->_socket.Send((*it)->get_fd(), msg);
    }
}

Client* Server::findClientByNick(const std::string& target)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second && it->second->getNickName() == target)
        {
            return (it->second);
        }
    }
    
    return (NULL);
}
