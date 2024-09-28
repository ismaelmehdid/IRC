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


bool    Server::nick(Client *client, const t_IRCCommand &command)
{
    if (!client->_has_set_password)
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", ERR_PASSWDMISMATCH)))
            return (false);
    } 
    else if (command.params.empty())
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", ERR_NEEDMOREPARAMS)))
            return (false);
    }
    else
    {
        std::string newNick = command.params[0];
        if (client->getNickName() == newNick)
            return (true);
        
        if (!is_nickname_valid(newNick))
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, newNick, ERR_ERRONEUSNICKNAME)))
                return (false);
        }
        else if (this->isNickNameTaken(newNick))
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, newNick, ERR_NICKNAMEINUSE)))
                return (false);
        }
        else
        {
            bool wasEmpty = client->getNickName().empty();
            client->setNickName(newNick);
            
            if (wasEmpty && client->is_authenticated())
            {
                if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", RPL_WELCOME)))
                    return (false);
                std::cout << GREEN << "Client on fd " << client->get_fd()
                          << " authenticated " << client->getPrefix() << RESET << std::endl;
                this->_nicknames.push_back(client->getNickName());
            }
        }
    }
    return (true);
}
