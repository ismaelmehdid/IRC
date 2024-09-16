#include "../../include/server/Server.hpp"

Channel* Server::createChannel(const std::string& channelName)
{
    if (_channels.find(channelName) == _channels.end())
    {
        Channel* new_channel = new Channel(channelName);
        _channels[channelName] = new_channel;
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


void Server::sendChannelMessage(const std::string& msg, const std::string& channelName)
{
    Channel* channel = findChannel(channelName);
    if (channel)
    {
        channel->broadcastMessage(msg);
    }
}

Client* Server::findClientByNick(const std::string& target)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNickName() == target)
        {
            return (it->second);
        }
    }
    
    return (NULL);
}
