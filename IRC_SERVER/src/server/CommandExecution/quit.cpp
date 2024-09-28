#include "../../../include/server/Server.hpp"

bool    Server::quit(Client *client, const t_IRCCommand &command)
{
	std::string reason = (command.trailing.empty()) ? "\r\n" : (":" + command.trailing + "\r\n");

    std::cout << YELLOW << client->getPrefix() << " disconnected!" << RESET << std::endl;

    this->removeClient(client, reason);

    return (true);
}