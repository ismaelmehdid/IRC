#include "../../../include/server/Server.hpp"

void    Server::privMsg(Client *client, const t_IRCCommand &command)
{
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

        std::string     privMsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        this->broadcastMessage(privMsg, channel);
    }
    else
    {
        Client *target_client = this->findClientByNick(target);
        if (!target_client)
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, target, ERR_NOSUCHNICK));
            return;
        }
        std::string privmsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        this->_socket.send(target_client->get_fd(), privmsg);
    }
}