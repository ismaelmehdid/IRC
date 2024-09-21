#include "../../../include/server/Server.hpp"

void Server::invite(Client *client, const t_IRCCommand &command)
{
    int                 fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, ERR_NOTREGISTERED);
        return ;
    }

    if (command.params.size() < 2)
    {
        this->_socket.send(fd, ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &channelName = command.params[1];
    const std::string   &clientName = command.params[0];
    Channel             *channelToInvite = this->findChannel(channelName);

    if (!channelToInvite)
    {
        this->_socket.send(fd, ERR_NO_SUCH_CHANNEL);
        return ;
    }

    Client *clientToInvite = this->findClientByNick(clientName);
    if (!clientToInvite)
    {
        // ERR_NOSUCHNICK 
        return ;
    }

    if (channelToInvite->isMember(clientToInvite))
    {
        // ERR_USERONCHANNEL 
        return ;
    }

    if (channelToInvite->isInvited(clientToInvite))
    {
        // ERR_ALREADYINVITED
        return ;
    }

    channelToInvite->addInvited(clientToInvite);

    this->_socket.send(fd, ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n");
    this->_socket.send(clientToInvite->get_fd(), ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n");
}
