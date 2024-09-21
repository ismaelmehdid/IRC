#include "../../../include/server/Server.hpp"

void    Server::part(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PART", ERR_NEEDMOREPARAMS));
        return;
    }

    const std::string   &message = command.trailing;
    const std::string   &channel_name = command.params[0];
    Channel             *channel = this->findChannel(channel_name);
    

    if (!channel)
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, channel_name, ERR_NOSUCHCHANNEL));
        return;
    }

    channel->removeClient(client);
    this->broadcastMessage(getMessage(client, NULL, channel, message, RAW_PART), channel);
}
