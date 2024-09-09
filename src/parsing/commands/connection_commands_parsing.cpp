#include "../../../include/irc.hpp"

enum commands {
    PASS,
    NICK,
    USER
}

void get_all_cmd_parts(const std::string &cmd, std::vector<std::string> &parts)
{
    std::stringstream ss(cmd);

    std::string part;

    while (ss >> part) {
        parts.push_back(part);
    }
}

void handle_connection_command(int client_fd, const std::string &full_command)
{
    std::vector<std::string> parts;

    get_all_cmd_parts(full_command, parts);

    if (parts.empty()) {
        return ;
    }

    std::string command = parts[0];
    
}