#include "../../../include/server/Server.hpp"

bool    Server::ping(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        if (!this->_socket.Send(client->get_fd(), getMessage(client, NULL, NULL, "PING", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    if (!this->_socket.Send(client->get_fd(), pong_message))
        return (false);
    return (true);
}