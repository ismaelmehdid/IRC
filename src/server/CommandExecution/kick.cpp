#include "../../../include/server/Server.hpp"

void    Server::kick(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, ERR_NOTREGISTERED);
        return;
    }

    if (command.params.size() < 2)
    {
        this->_socket.send(fd, ERR_NEED_MORE_PARAMS);
        return;
    }

    std::string channelName = command.params[0];
    std::string clientName = command.params[1];
    std::string kickMessage = command.trailing.empty() ? command.trailing : client->getNickName();

    Channel *channelToKickFrom = this->findChannel(channelName);
    if (!channelToKickFrom)
    {
        this->_socket.send(fd, ERR_NO_SUCH_CHANNEL);
        return;
    }

    if (!channelToKickFrom->isOperator(client))
    {
        this->_socket.send(fd, getMessage(client, ERR_CHANOPRIVSNEEDED, channelToKickFrom));
        return;
    }

    Client *clientToKick = this->findClientByNick(clientName);
    if (!clientToKick)
    {
        this->_socket.send(fd, getMessage(client, ERR_NOSUCHNICK, channelToKickFrom));
        return;
    }

    if (!channelToKickFrom->isMember(clientToKick))
    {
        this->_socket.send(fd, getMessage(client, ERR_USERNOTINCHANNEL, channelToKickFrom));
        return;
    }

    this->broadcastMessage(client->getPrefix() + " KICK " + channelName + " " + clientName + " :" + kickMessage + "\r\n", channelToKickFrom); // raw🤮
    channelToKickFrom->removeClient(clientToKick);
    // ABOVE KICK EXAMPLE: ":dan!d@localhost KICK #Melbourne alice :dan"
}