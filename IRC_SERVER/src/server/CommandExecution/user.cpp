#include "../../../include/server/Server.hpp"

void    Server::user(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated() && client->getUserName().empty() && client->getFullName().empty())
    {
        if (!client->_has_set_password)
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "USER", ERR_PASSWDMISMATCH));
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PRIVMSG", ERR_NEEDMOREPARAMS));
        }
        else
        {
            client->setUserName(command.params[0]);
            client->setFullName(command.trailing);

            if (client->is_authenticated())
            {
                this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", RPL_WELCOME));
                std::cout << GREEN << "Client on fd " << client->get_fd()
                          << " authenticated " << client->getPrefix() << RESET << std::endl;
                this->_nicknames.push_back(client->getNickName());
            }
        }
    }
    else
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "USER", ERR_ALREADYREGISTERED));
    }
}