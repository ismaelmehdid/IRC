#include "../../include/server/Server.hpp"

void    Client::executeKick(const t_IRCCommand &command)
{
    this->_role->kick(command);
}

void    Client::executeInvite(const t_IRCCommand &command)
{
    this->_role->invite(command);
}

void    Client::executeTopic(const t_IRCCommand &command)
{
    this->_role->topic(command);
}

void    Client::executeMode(const t_IRCCommand &command)
{
    this->_role->mode(command);
}

void    Client::executeCap(const t_IRCCommand &command)
{
    this->_role->cap(command);
}

void    Client::executePass(const t_IRCCommand &command)
{
    this->_role->pass(command);
}

void    Client::executeNick(const t_IRCCommand &command)
{
    this->_role->nick(command);
}

void    Client::executeUser(const t_IRCCommand &command)
{
    this->_role->user(command);
}

void    Client::executeJoin(const t_IRCCommand &command)
{
    this->_role->join(command);
}

void    Client::executePart(const t_IRCCommand &command)
{
    this->_role->part(command);
}

void    Client::executePrivMsg(const t_IRCCommand &command)
{
    this->_role->privMsg(command);
}

void    Client::executePing (const t_IRCCommand &command)
{
    this->_role->ping(command);
}

void    Client::executeQuit (const t_IRCCommand &command)
{
    this->_role->quit(command);
}

/**
 * Executes the given client command.
 *
 * This function parses the provided message into a vector of IRC commands and iterates over each command.
 * For each command, it looks up the corresponding command function in the _commandMap and executes it.
 * If the command is not found in the map, it sends the ERR_UNKNOWNCOMMAND response to the client.
 *
 * @param message The message containing the client command.
 */
void    Client::executeCommand(const std::string &message)
{
    std::vector<t_IRCCommand> parsed_commands = parseRequests(message);

    for (size_t i = 0; i < parsed_commands.size(); i++)
    {
        std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
        if (it != _commandMap.end())
        {
            (this->*(it->second))(parsed_commands[i]);
        }
        else
        {
            global_ircserv->socketSend(_fd, ERR_UNKNOWNCOMMAND);
        }
    }
}