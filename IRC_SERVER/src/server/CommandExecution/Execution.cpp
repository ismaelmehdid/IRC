#include "../../../include/server/Server.hpp"

bool    Server::executeCommand(Client* client, const std::string &message)
{
    try
    {
        std::vector<t_IRCCommand>   parsed_commands = parseRequests(message);

        for (size_t i = 0; i < parsed_commands.size(); i++)
        {
            std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
            
            if (it != _commandMap.end())
            {
                CommandFunction commandFunction = it->second;
                if (!(this->*commandFunction)(client, parsed_commands[i]))
                    return (false);
            }
            else
            {
                if (!this->_socket.Send(client->get_fd(), global_ircserv->getMessage(client, NULL, NULL, parsed_commands[i].command, ERR_UNKNOWNCOMMAND)))
                    return (false);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return (true);
}
