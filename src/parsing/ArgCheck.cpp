#include "../../include/irc.hpp"

/**
 * @brief Checks if a given port is valid.
 * 
 * This function checks if the provided port is valid by performing the following checks:
 * - The port must not be empty.
 * - The port length must be between 4 and 5 characters.
 * - All characters in the port must be digits.
 * - The port value must be between 1024 and 65535 (inclusive).
 * 
 * @param port The port to be checked.
 * @return true if the port is valid, false otherwise.
 */
static bool checkPort(const std::string &port)
{
    size_t len = port.size();

    if (!port.size())
        return (false);

    if (len < 4 || len > 5)
        return (false);

    for (size_t i = 0; i < len; ++i)
    {
        if (!std::isdigit(port[i]))
            return (false);
    }

    char        *endptr;
    long int    portCheck = std::strtol(port.c_str(), &endptr, 10);

    if (*endptr != '\0' || portCheck < MIN_PORT_VALUE || portCheck > MAX_PORT_VALUE)
        return (false);

    return (true);
}

/**
 * @brief Parses the command line arguments and performs necessary checks.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 */
void    validateArguments(int argc, char **argv)
{
    if (argc != ARGUMENTS_REQUIRED)
    {
        display_error_message(ERR_BAD_ARGUMENTS);
        std::exit(EXIT_FAILURE);
    }

    if (!checkPort((argv[1])))
    {
        display_error_message(ERR_BAD_PORT);
        std::exit(EXIT_FAILURE);
    }
}