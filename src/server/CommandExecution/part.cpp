#include "../../../include/server/Server.hpp"

void    Server::part(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NOTREGISTERED));
        return ;
    }

    if (command.params.empty())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NEEDMOREPARAMS));
        return ;
    }

    const std::string   &channelName = command.params[0];
    Channel             *channel = this->findChannel(channelName);
    

    if (!channel)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL));
        return ;
    }

    if (!channel->isMember(client))
    {
        this->_socket.send(fd, getMessage(client, NULL, channel, "PART", ERR_USERNOTINCHANNEL));
        return ;
    }

    std::string         part_message = ":" + client->getPrefix() + " PART " + channelName;
    if (!command.trailing.empty())
    {
        part_message += " :" + command.trailing;
    }
    part_message += "\r\n";

    this->broadcastMessage(part_message, channel);
    channel->removeClient(client);

    if (channel->getNbrUsers() == 0)
    {
        this->removeChannel(channel);
    }
}
