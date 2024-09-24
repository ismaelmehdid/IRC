#include "../../../include/server/Server.hpp"

void Server::invite(Client *client, const t_IRCCommand &command)
{
    int                 fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NOTREGISTERED));
        return ;
    }

    if (command.params.size() < 2)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "KICK", ERR_NEEDMOREPARAMS));
        return ;
    }

    const std::string   &channelName = command.params[1];
    const std::string   &clientName = command.params[0];
    Channel             *channelToInvite = this->findChannel(channelName);

    if (!channelToInvite)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL));
        return ;
    }

    Client *clientToInvite = this->findClientByNick(clientName);
    if (!clientToInvite)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, clientName, ERR_NOSUCHNICK));
        return ;
    }

    if (channelToInvite->isMember(clientToInvite))
    {
        this->_socket.send(fd, getMessage(client, clientToInvite, channelToInvite, "INVITE", ERR_USERONCHANNEL));
        return ;
    }

    if (channelToInvite->isInvited(clientToInvite))
    {
        // no need to return an error apparently
        return ;
    }

    channelToInvite->addInvited(clientToInvite);

    this->_socket.send(fd, ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n");
    this->_socket.send(clientToInvite->get_fd(), ":" + client->getNickName() + " INVITE " + clientToInvite->getNickName() + " " + channelName + "\r\n");
}
