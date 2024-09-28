#include "../../../include/server/Server.hpp"

bool    Server::pass(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated())
    {
        if (command.params.empty())
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_NEEDMOREPARAMS)))
                return (false);
        }
        else if (command.params[0] != this->_password)
        {
            if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_PASSWDMISMATCH)))
                return (false);
        }
        else
        {
            client->_has_set_password = true;
        }
    }
    else
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_ALREADYREGISTERED)))
            return (false);
    }
    return (true);
}