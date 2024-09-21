#include "../../../include/server/Server.hpp"

// void Server::PermissionDenied(Client* client, const std::string &nickName)
// {
//     this->_socket.send(client->get_fd(), ":server 481 " + nickName + ": Permission Denied - You're not an IRC operator.\r\n");
// }

void    Server::executeCommand(Client* client, const std::string &message)
{
    try
    {
        std::vector<t_IRCCommand>   parsed_commands = parseRequests(message);

        for (size_t i = 0; i < parsed_commands.size(); i++)
        {
            std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
            
            if (it != _commandMap.end())
            {
                (this->*(it->second))(client, parsed_commands[i]);
            }
            else
            {
                this->_socket.send(client->get_fd(), global_ircserv->getMessage(client, NULL, NULL, parsed_commands[i].command, ERR_UNKNOWNCOMMAND));
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
