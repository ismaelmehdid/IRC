#include "../../../include/server/Server.hpp"

void    Server::quit(Client *client, const t_IRCCommand &command)
{
	std::string reason = (command.params.empty()) ? "" : (":" + command.params[0]);

    std::cout << YELLOW << client->getNickName() << " disconnected!" << RESET << std::endl;

	this->_socket.send(client->get_fd(), getMessage(client, NULL, NULL, reason, RAW_QUIT));
    this->removeClient(client, reason);
}