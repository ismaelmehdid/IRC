#include "../../include/server/Server.hpp"

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

void    Client::executeKick(const t_IRCCommand &command)
{
    global_ircserv->kick(this, command);
}

void    Client::executeInvite(const t_IRCCommand &command)
{
    global_ircserv->invite(this, command);
}

void    Client::executeTopic(const t_IRCCommand &command)
{
    global_ircserv->topic(this, command);
}

void    Client::executeMode(const t_IRCCommand &command)
{
    global_ircserv->mode(this, command);
}

void    Client::executeCap(const t_IRCCommand &command)
{
    global_ircserv->cap(this, command);
}

void    Client::executePass(const t_IRCCommand &command)
{
    global_ircserv->pass(this, command);
}

void    Client::executeNick(const t_IRCCommand &command)
{
    global_ircserv->nick(this, command);
}

void    Client::executeUser(const t_IRCCommand &command)
{
    global_ircserv->user(this, command);
}

void    Client::executeJoin(const t_IRCCommand &command)
{
    global_ircserv->join(this, command);
}

void    Client::executePart(const t_IRCCommand &command)
{
    global_ircserv->part(this, command);
}

void    Client::executePrivMsg(const t_IRCCommand &command)
{
    global_ircserv->privMsg(this, command);
}

void    Client::executePing (const t_IRCCommand &command)
{
    global_ircserv->ping(this, command);
}

void    Client::executeQuit (const t_IRCCommand &command)
{
    global_ircserv->quit(this, command);
}
