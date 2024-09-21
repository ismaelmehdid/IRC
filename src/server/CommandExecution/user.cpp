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
            std::cout << GREEN << "Client connected!" << RESET << std::endl;

            if (client->is_authenticated())
            {
                this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "NICK", RPL_WELCOME));
            }
        }
    }
    else
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "USER", ERR_ALREADYREGISTERED));
    }
}