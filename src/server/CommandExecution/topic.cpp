#include "../../../include/server/Server.hpp"

void    Server::topic(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "TOPIC", ERR_NOTREGISTERED));
        return ;
    }

    if (command.params.size() < 1)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "TOPIC", ERR_NEEDMOREPARAMS));
        return ;
    }

    Channel *channel = global_ircserv->findChannel(command.params[0]);
    
    if (!channel)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, command.params[0], ERR_NOSUCHCHANNEL));
        return ;
    }

    if (!channel->isMember(client))
    {
        this->_socket.send(fd, getMessage(client, NULL, channel, "TOPIC", ERR_NOTONCHANNEL));
        return ;
    }

    const std::string   &newTopic = command.trailing;

    if (newTopic.empty()) // display the topic
    {
        if (channel->getTopic().empty())
        {
            this->_socket.send(fd, getMessage(client, NULL, channel, "TOPIC", RPL_NOTOPIC));
        }
        else
        {
            this->_socket.send(fd, getMessage(client, NULL, channel, "TOPIC", RPL_TOPIC));
        }
    }
    else // change the topic
    {
        if (channel->isTopicLocked() && !channel->isOperator(client))
        {
            this->_socket.send(fd, getMessage(client, NULL, channel, "TOPIC", ERR_CHANOPRIVSNEEDED));
        }
        else
        {
            if (newTopic.size() > MAX_TOPIC_LENGTH)
            {
                return ;
            }
            else
            {
                channel->setTopic(newTopic);
                this->broadcastMessage(getMessage(client, NULL, channel, "TOPIC", RAW_TOPIC), channel);
            }
        }
    }
}