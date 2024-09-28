#include "../../../include/server/Server.hpp"

bool    Server::part(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    if (command.params.empty())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    const std::string   &channelName = command.params[0];
    Channel             *channel = this->findChannel(channelName);
    

    if (!channel)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL)))
            return (false);
        return (true);
    }

    if (!channel->isMember(client))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "PART", ERR_NOTONCHANNEL)))
            return (false);
        return (true);
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
    return (true);
}
