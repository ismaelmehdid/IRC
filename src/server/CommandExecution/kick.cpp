#include "../../../include/server/Server.hpp"

void    Server::kick(Client *client, const t_IRCCommand &command)
{
    // don't forget to check in commands that a user is registered
    (void)client;
    (void)command;
}