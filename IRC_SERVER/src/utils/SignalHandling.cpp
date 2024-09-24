#include "../../include/server/Server.hpp"

void    handleShuttingDown(int sig)
{
    std::cout << "Caught signal " << sig << ", shutting down server..." << std::endl;
    if (global_ircserv != NULL)
    {
        delete (global_ircserv);
    }
    std::exit(0);
}