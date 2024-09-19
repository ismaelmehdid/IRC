#include "../../../include/server/Server.hpp"

static bool is_nickname_valid(const std::string &nick)
{
    if (nick.size() > MAX_NICKNAME_LENGTH)
        return (false);
    
    if (std::isdigit(nick[0]) || nick[0] == '#' || nick[0] == '&')
        return (false);
    
    for (size_t i = 1; i < nick.size(); i++)
    {
        if (!std::isalnum(nick[0]) &&
            nick[0] != '-' &&
            nick[0] != '[' &&
            nick[0] != ']' &&
            nick[0] != '\\' &&
            nick[0] != '^' &&
            nick[0] != '_' &&
            nick[0] != '{' &&
            nick[0] != '}'
            )
            return (false);
    }
    return (true);
}


void    Server::nick(Client *client, const t_IRCCommand &command)
{
    if (!client->_has_set_password)
    {
        this->_socket.send(client->get_fd(), ERR_PASSWORD_REQUIRED);
    } 
    else if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), ERR_NEED_MORE_PARAMS);
    }
    else
    {
        std::string newNick = command.params[0];
        if (client->getNickName() == newNick)
            return ;
        if (!is_nickname_valid(newNick))
        {
            this->_socket.send(client->get_fd(), ":server 432 * " + newNick + " :Erroneous nickname\r\n");
        }
        else if (this->isNickNameTaken(newNick))
        {
            this->_socket.send(client->get_fd(), ERR_NICKNAME_IN_USE);
        }
        else
        {
            // TODO: broadcast new nickname to all users in channel
            bool wasEmpty = client->getNickName().empty();
            client->setNickName(newNick);
            if (wasEmpty && client->is_authenticated())
            {
                this->_socket.send(client->get_fd(),
                    client->getPrefix() + WELCOME_CODE + client->getNickName() + MSG_WELCOME);
            }
        }
    }
}