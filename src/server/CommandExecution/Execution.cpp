#include "../../../include/server/Server.hpp"

// void Server::PermissionDenied(Client* client, const std::string &nickName)
// {
//     this->_socket.send(client->get_fd(), ":server 481 " + nickName + ": Permission Denied - You're not an IRC operator.\r\n");
// }

void    Server::executeCommand(Client* client, const std::string &message)
{
    std::vector<t_IRCCommand> parsed_commands = parseRequests(message);

    for (size_t i = 0; i < parsed_commands.size(); i++)
    {
        std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
        if (it != _commandMap.end())
        {
            // Call the command function, passing the client and the parsed command
            (this->*(it->second))(client, parsed_commands[i]);
        }
        else
        {
            this->_socket.send(client->get_fd(), global_ircserv->getMessage(client, NULL, NULL, parsed_commands[i].command, ERR_UNKNOWNCOMMAND));
        }
    }
}
