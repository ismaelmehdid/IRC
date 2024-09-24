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
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", ERR_PASSWDMISMATCH));
    } 
    else if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", ERR_NEEDMOREPARAMS));
    }
    else
    {
        std::string newNick = command.params[0];
        if (client->getNickName() == newNick)
            return ;
        
        if (!is_nickname_valid(newNick))
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, newNick, ERR_ERRONEUSNICKNAME));
        }
        else if (this->isNickNameTaken(newNick))
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, newNick, ERR_NICKNAMEINUSE));
        }
        else
        {
            bool wasEmpty = client->getNickName().empty();
            client->setNickName(newNick);
            
            if (wasEmpty && client->is_authenticated())
            {
                this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", RPL_WELCOME));
                std::cout << GREEN << "Client on fd " << client->get_fd()
                          << " authenticated " << client->getPrefix() << RESET << std::endl;
            }
        }
    }
}
