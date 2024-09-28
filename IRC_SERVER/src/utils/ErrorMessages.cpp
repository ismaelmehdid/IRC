#include "../../include/irc.hpp"


/**
 * @brief Displays an error message based on the provided error code.
 * 
 * This function takes an error code of type `t_errors` and prints a corresponding
 * error message to the standard error stream. The error messages are color-coded
 * for better readability.
 * 
 * @param code The error code indicating the type of error.
 * 
 * Error codes and their corresponding messages:
 * - ERR_BAD_ARGUMENTS: Indicates an invalid number of arguments. Provides usage
 *   instructions and an example.
 * - ERR_BAD_PORT: Indicates an invalid port number. Specifies the allowed port range.
 */
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
    case ERR_BAD_PORT:
        std::cerr << RED << "Error: Invalid port.\n"
                  << "Allowed ports are: 1024 < port < 65535" << RESET << std::endl;
        break; 
    default:
        break;
    }
}
