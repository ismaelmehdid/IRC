#include "../../../include/server/Server.hpp"

void    Server::part(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return;
    }

    const std::string   &channel_name = command.params[0];
    Channel             *channel = this->findChannel(channel_name);
    
    if (!channel)
    {
        this->_socket.send(client->get_fd(), ERR_NO_SUCH_CHANNEL);
        return;
    }

    channel->removeClient(client);
    std::string         part_message = ":" + client->getNickName() + " PART " + channel_name + "\r\n";
    this->broadcastMessage(part_message, channel);
}