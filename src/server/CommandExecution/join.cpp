#include "../../../include/server/Server.hpp"

/*static bool IsChannelNameValid(const std::string &name)
{
    (void)name;
    return (true);
}*/

void    Server::join(Client *client, const t_IRCCommand &command)
{
    int     fd = client->get_fd();

    if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    Channel *channel = this->findChannel(command.params[0]);

    if (!channel)
    {
        channel = this->createChannel(command.params[0]);
    }

    // checks for invite only password etc
    if (!channel->isMember(client))
    {
        channel->addClient(client);
        this->broadcastMessage(getMessage(client, JOIN, channel), channel);

        if (!channel->getTopic().empty())
        {
            this->_socket.send(fd, getMessage(client, TOPIC, channel));
        }
        else
        {
            this->_socket.send(fd, getMessage(client, NO_TOPIC, channel));
        }

        this->_socket.send(fd, getMessage(client, NAMES_REPLY, channel));
        this->_socket.send(fd, getMessage(client, END_OF_NAMES, channel));
        // mode msg
    }
    else
    {
        this->_socket.send(fd, getMessage(client, ALREADY_JOINED_ERROR, channel));
    }
}