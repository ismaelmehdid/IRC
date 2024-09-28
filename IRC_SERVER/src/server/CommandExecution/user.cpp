#include "../../../include/server/Server.hpp"

bool    Server::user(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated() && client->getUserName().empty() && client->getFullName().empty())
    {
        if (!client->_has_set_password)
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "USER", ERR_PASSWDMISMATCH)))
                return (false);
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PRIVMSG", ERR_NEEDMOREPARAMS)))
                return (false);
        }
        else
        {
            client->setUserName(command.params[0]);
            client->setFullName(command.trailing);

            if (client->is_authenticated())
            {
                if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", RPL_WELCOME)))
                    return (false);
                std::cout << GREEN << "Client on fd " << client->get_fd()
                          << " authenticated " << client->getPrefix() << RESET << std::endl;
                this->_nicknames.push_back(client->getNickName());
            }
        }
    }
    else
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "USER", ERR_ALREADYREGISTERED)))
            return (false);
    }
    return (true);
}