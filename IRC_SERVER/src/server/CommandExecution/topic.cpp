#include "../../../include/server/Server.hpp"

bool    Server::topic(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "TOPIC", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    if (command.params.size() < 1)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "TOPIC", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    Channel *channel = global_ircserv->findChannel(command.params[0]);
    
    if (!channel)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, command.params[0], ERR_NOSUCHCHANNEL)))
            return (false);
        return (true);
    }

    if (!channel->isMember(client))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "TOPIC", ERR_NOTONCHANNEL)))
            return (false);
        return (true);
    }

    const std::string   &newTopic = command.trailing;

    if (newTopic.empty()) // display the topic
    {
        if (channel->getTopic().empty())
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "TOPIC", RPL_NOTOPIC)))
                return (false);
        }
        else
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "TOPIC", RPL_TOPIC)))
                return (false);
        }
    }
    else // change the topic
    {
        if (channel->isTopicLocked() && !channel->isOperator(client))
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "TOPIC", ERR_CHANOPRIVSNEEDED)))
                return (false);
        }
        else
        {
            if (newTopic.size() > MAX_TOPIC_LENGTH)
            {
                return (true);
            }
            else
            {
                channel->setTopic(newTopic);
                this->broadcastMessage(getMessage(client, NULL, channel, "TOPIC", RAW_TOPIC), channel);
            }
        }
    }
    return (true);
}