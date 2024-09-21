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
        
        this->_socket.send((*it)->get_fd(), msg);
    }
}

void    Server::privMsg(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "PRIVMSG", ERR_NOTREGISTERED));
        return ;
    }

    if (command.params.empty() || command.trailing.empty())
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PRIVMSG", ERR_NEEDMOREPARAMS));
        return ;
    }

    const std::string   &target = command.params[0];
    const std::string   &message = command.trailing;

    if (target[0] == '#')
    {
        Channel *channel = this->findChannel(target);
        
        if (!channel)
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, target, ERR_NOSUCHCHANNEL));
            return ;
        }

        if (!channel->isMember(client))
        {
            this->_socket.send(fd, getMessage(client, NULL, channel, target, ERR_USERNOTINCHANNEL));
            return ;
        }

        std::string     privMsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        this->privMsgBroadcast(privMsg, channel, fd);
    }
    else
    {
        Client  *target_client = this->findClientByNick(target);
        
        if (!target_client)
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, target, ERR_NOSUCHNICK));
            return;
        }

        std::string privmsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        
        this->_socket.send(target_client->get_fd(), privmsg);
    }
}