#include "../../include/irc.hpp"

static t_IRCCommand parse_single_command(const std::string &raw_command)
{
    t_IRCCommand command;
    size_t pos = 0;

    if (raw_command[0] == ':')
    {
        pos = raw_command.find(' ');
        command.prefix = raw_command.substr(1, pos - 1);
        pos++;
    }

    size_t space_pos = raw_command.find(' ', pos);
    command.command = raw_command.substr(pos, space_pos - pos);
    pos = space_pos + 1;

    while (pos < raw_command.length())
    {
        if (raw_command[pos] == ':')
        {
            command.trailing = raw_command.substr(pos + 1);
            break ;
        }

        space_pos = raw_command.find(' ', pos);
        if (space_pos == std::string::npos)
        {
            command.params.push_back(raw_command.substr(pos));
            break ;
        }
        else
        {
            command.params.push_back(raw_command.substr(pos, space_pos - pos));
            pos = space_pos + 1;
        }
    }

    return (command);
}


std::vector<t_IRCCommand>   parse_client_commands(const std::string &commands)
{

    std::vector<t_IRCCommand>   extracted;
    size_t                      start = 0;
    size_t                      end;

    while ((end = commands.find("\r\n", start)) != std::string::npos)
    {
        std::string     command = commands.substr(start, end - start);
        t_IRCCommand    parsed_command = parse_single_command(command);

        extracted.push_back(parsed_command);
        start = end + 2;
    }

    return (extracted);
}
