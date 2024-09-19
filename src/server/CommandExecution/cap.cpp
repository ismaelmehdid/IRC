#include "../../../include/server/Server.hpp"

void    Server::cap(Client *client, const t_IRCCommand &command)
{
	if (command.params[0] == "LS")
	{
         // server doesn't have any special features
        std::string empty_list_response = ":server CAP " + client->getNickName() + "LS :\r\n";
        this->_socket.send(client->get_fd(), empty_list_response);
	}
}