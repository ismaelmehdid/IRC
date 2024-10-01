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

std::string trimMessage(const std::string &message)
{
    std::string trimmedMessage = message;
    while (!trimmedMessage.empty() && \
        (trimmedMessage[trimmedMessage.size() - 1] == '\r' || \
            trimmedMessage[trimmedMessage.size() - 1] == '\n'))
    {
        trimmedMessage.erase(trimmedMessage.size() - 1);
    }
    return (trimmedMessage);
}

void    displayUserMessage(const std::string &nickName, const std::string &message)
{
    std::string cleanMessage = trimMessage(message);
    bool        hasNick = !nickName.empty();
    std::string displayedNick = hasNick ? nickName : "Unknown";
    int         messageWidth = cleanMessage.length();
    int         nickWidth = displayedNick.length();
    int         totalWidth = nickWidth + messageWidth + 7;

    std::cout << BG_BLUE << WHITE << std::setw(totalWidth) << std::setfill('-') << "" << RESET << std::endl;

    std::cout << BG_BLUE << WHITE << "| " 
              << std::setw(nickWidth) << std::left << "Nickname" << " | "
              << std::setw(messageWidth) << std::left << "Message" << " |" 
              << RESET << std::endl;

    std::cout << BG_BLUE << WHITE << std::setw(totalWidth) << std::setfill('-') << "" << RESET << std::endl;

    std::cout << BG_BLUE << WHITE << "| " 
              << std::setw(nickWidth) << std::left << displayedNick << " | "
              << std::setw(messageWidth) << std::left << cleanMessage << " |" 
              << RESET << std::endl;

    std::cout << BG_BLUE << WHITE << std::setw(totalWidth) << std::setfill('-') << "" << RESET << std::endl << std::endl;
    std::cout.flush();
}
