#include "../../../include/server/Server.hpp"

bool    Server::cap(Client *client, const t_IRCCommand &command)
{
	if (command.params[0] == "LS")
	{
                // server doesn't have any special features
                std::string empty_list_response = ":server CAP " + client->getNickName() + "LS :\r\n";
                if (!this->_socket.Send(client->get_fd(), empty_list_response))
                        return (false);
	}
        return (true);
}