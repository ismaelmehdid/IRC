#include "../../include/irc.hpp"

void display_error_message(t_errors code)
{
    switch (code)
    {
    case ERR_BAD_ARGUMENTS:
        std::cerr << RED << "Error: Invalid number of arguments.\n"
                  << "Usage: ./ircserv <listeningPort> <connectionPassword>\n"
                  << "Example: ./ircserv 6667 mypassword\n"
                  << "Please ensure you provide the correct number of arguments and try again." << RESET << std::endl;
        break;
    default:
        break;
    }
}
