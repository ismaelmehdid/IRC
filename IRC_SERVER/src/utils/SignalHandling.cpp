#include "../../include/server/Server.hpp"

/**
 * @brief Handles the shutdown process when a signal is caught.
 *
 * This function is called when a specific signal is received by the server.
 * It logs the signal number and initiates the server shutdown process.
 * If the global IRC server instance is not null, it deletes the instance
 * to free up resources before exiting the program.
 *
 * @param sig The signal number that was caught.
 */
void    handleShuttingDown(int sig)
{
    std::cout << "Caught signal " << sig << ", shutting down server..." << std::endl;
    if (global_ircserv != NULL)
    {
        delete (global_ircserv);
    }
    std::exit(0);
}