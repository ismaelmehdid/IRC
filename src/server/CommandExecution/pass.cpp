#include "../../../include/server/Server.hpp"

void    Server::pass(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated())
    {
        if (command.params.empty())
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_NEEDMOREPARAMS));
        }
        else if (command.params[0] != this->_password)
        {
            this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_PASSWDMISMATCH));
        }
        else
        {
            client->_has_set_password = true;
            //TODO: Notify that the client have put the right password
        }
    }
    else
    {
        this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, "PASS", ERR_ALREADYREGISTERED));
    }
}