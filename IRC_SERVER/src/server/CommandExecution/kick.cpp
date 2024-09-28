#include "../../../include/server/Server.hpp"

bool    Server::kick(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

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

    std::string channelName = command.params[0];
    std::string clientName  = command.params[1];
    std::string kickMessage = command.trailing.empty() ? command.trailing : client->getNickName();
    Channel     *channelToKickFrom = this->findChannel(channelName);
    
    if (!channelToKickFrom)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL)))
            return (false);
        return (true);
    }

    if (!channelToKickFrom->isOperator(client))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToKickFrom, "KICK", ERR_CHANOPRIVSNEEDED)))
            return (false);
        return (true);
    }

    Client      *clientToKick = this->findClientByNick(clientName);
    
    if (!clientToKick)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToKickFrom, clientName, ERR_NOSUCHNICK)))
            return (false);
        return (true);
    }

    if (!channelToKickFrom->isMember(clientToKick))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToKickFrom, "KICK", ERR_NOTONCHANNEL)))
            return (false);
        return (true);
    }

    this->broadcastMessage(clientToKick->getPrefix() + " KICK " + channelName + " " + clientName + " :" + kickMessage + "\r\n", channelToKickFrom);
    
    channelToKickFrom->removeClient(clientToKick);

    if (channelToKickFrom->getNbrUsers() == 0)
    {
        this->removeChannel(channelToKickFrom); // delete channel if it's empty
    }
    
    return (true);
}
