#include "../../../include/server/Server.hpp"

bool    Server::invite(Client *client, const t_IRCCommand &command)
{
    int                 fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    if (command.params.size() < 2)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    const std::string   &channelName = command.params[1];
    const std::string   &clientName = command.params[0];
    Channel             *channelToInvite = this->findChannel(channelName);

    if (!channelToInvite)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL)))
            return (false);
        return (true);
    }

    Client *clientToInvite = this->findClientByNick(clientName);
    if (!clientToInvite)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, clientName, ERR_NOSUCHNICK)))
            return (false);
        return (true);
    }

    if (channelToInvite->isMember(clientToInvite))
    {
        if (!this->_socket.Send(fd, getMessage(client, clientToInvite, channelToInvite, "INVITE", ERR_USERONCHANNEL)))
            return (false);
        return (true);
    }

    if (channelToInvite->isInvited(clientToInvite))
        return (true); // no need to return an error apparently

    channelToInvite->addInvited(clientToInvite);

    if (!this->_socket.Send(fd, ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n"))
        return (false);
    if (!this->_socket.Send(clientToInvite->get_fd(), ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n"))
        return (false);
    return (true);
}
