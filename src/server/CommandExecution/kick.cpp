#include "../../../include/server/Server.hpp"

void    Server::kick(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

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

    std::string channelName = command.params[0];
    std::string clientName  = command.params[1];
    std::string kickMessage = command.trailing.empty() ? command.trailing : client->getNickName();
    Channel     *channelToKickFrom = this->findChannel(channelName);
    
    if (!channelToKickFrom)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL));
        return ;
    }

    if (!channelToKickFrom->isOperator(client))
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToKickFrom, "KICK", ERR_CHANOPRIVSNEEDED));
        return ;
    }

    Client      *clientToKick = this->findClientByNick(clientName);
    if (!clientToKick)
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToKickFrom, clientName, ERR_NOSUCHNICK));
        return ;
    }

    if (!channelToKickFrom->isMember(clientToKick))
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToKickFrom, "KICK", ERR_NOTONCHANNEL));
        return ;
    }

    this->broadcastMessage
    (clientToKick->getPrefix() + " KICK " + channelName + " " + clientName + " :" + kickMessage + "\r\n", channelToKickFrom); // raw
    
    channelToKickFrom->removeClient(clientToKick);

    if (channelToKickFrom->getNbrUsers() == 0)
    {
        this->removeChannel(channelToKickFrom); // delete channel if it's empty
    }
}
