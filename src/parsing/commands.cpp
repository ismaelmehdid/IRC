#include "../../include/irc.hpp"

t_IRCCommand parse_IRC_command(const std::string &command)
{
    t_IRCCommand ircMessage;

    std::istringstream  iss(command);
    std::string         token;

    if (command[0] == ':') {
        iss >> ircMessage.prefix;
        ircMessage.prefix = ircMessage.prefix.substr(1);
    }

    iss >> ircMessage.command;
    std::cout << "Parsing command: " << ircMessage.command << std::endl;

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