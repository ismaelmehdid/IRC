#include "../../../include/server/Server.hpp"

void    Server::ping(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        this->_socket.send(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    this->_socket.send(client->get_fd(), pong_message);
}