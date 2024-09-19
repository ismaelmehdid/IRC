#include "../../../include/server/Server.hpp"

void    Server::user(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated() && client->getUserName().empty() && client->getFullName().empty())
    {
        if (!client->_has_set_password)
        {
            this->_socket.send(client->get_fd(), ERR_PASSWORD_REQUIRED);
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            this->_socket.send(client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else
        {
            client->setUserName(command.params[0]);
            client->setFullName(command.trailing);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;

            if (client->is_authenticated())
            {
                this->_socket.send(client->get_fd(),
                    client->getPrefix() + WELCOME_CODE + client->getNickName() + MSG_WELCOME);
            }
        }
    }
    else
    {
        this->_socket.send(client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}