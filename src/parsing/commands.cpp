#include "../../include/irc.hpp"

static t_IRCCommand parse_single_command(const std::string &command)
{
    t_IRCCommand ircMessage;

    std::istringstream  iss(command);
    std::string         token;

    if (command[0] == ':') {
        iss >> ircMessage.prefix;
        ircMessage.prefix = ircMessage.prefix.substr(1);
    }

    iss >> ircMessage.command;

    while (iss >> token) {
        if (token[0] == ':') {
            ircMessage.trailing = token.substr(1);
            std::string rest_of_command;
            std::getline(iss, rest_of_command);
            ircMessage.trailing += rest_of_command;
            break;
        } else {
            ircMessage.params.push_back(token);
        }
    }
    return ircMessage;
}

std::vector<t_IRCCommand>   parse_client_commands(const std::string &commands)
{

    std::vector<t_IRCCommand>   extracted;
    size_t                      start = 0;
    size_t                      end;

    while ((end = commands.find("\r\n", start)) != std::string::npos) {
        std::string command = commands.substr(start, end - start);
        t_IRCCommand parsed_command = parse_single_command(command);
        extracted.push_back(parsed_command);
        start = end + 2;
    }

    return extracted;
}
