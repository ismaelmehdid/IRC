#include "../../../include/server/Server.hpp"

void    Server::privMsgBroadcast(const std::string& msg, Channel* channel, int sender_fd)
{
    if (msg.empty())
    {
        std::cerr << "Attempted to broadcast an empty message in channel " << channel->getName() << std::endl;
        return ;
    }
    
    const std::set<Client*>&    clients = channel->getClients();

    for (std::set<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (sender_fd == (*it)->get_fd())
            continue ;
        
        this->_socket.Send((*it)->get_fd(), msg);
    }
}

bool    Server::privMsg(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "PRIVMSG", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    if (command.params.empty() || command.trailing.empty())
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PRIVMSG", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    const std::string   &target = command.params[0];
    const std::string   &message = command.trailing;

    if (target[0] == '#')
    {
        Channel *channel = this->findChannel(target);
        
        if (!channel)
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, target, ERR_NOSUCHCHANNEL)))
                return (false);
            return (true);
        }

        if (!channel->isMember(client))
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, target, ERR_NOTONCHANNEL)))
                return (false);
            return (true);
        }

        std::string     privMsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        this->privMsgBroadcast(privMsg, channel, fd);
    }
    else
    {
        Client  *target_client = this->findClientByNick(target);
        
        if (!target_client)
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, target, ERR_NOSUCHNICK)))
                return (false);
            return (true);
        }
        
        std::string privmsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        
        if (!this->_socket.Send(target_client->get_fd(), privmsg))
            return (false);
    }
    return (true);
}