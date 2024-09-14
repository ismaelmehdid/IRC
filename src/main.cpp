#include "../include/server/Server.hpp"

Server  *global_ircserv = NULL;

static void handleShuttingDown(int sig)
{
    std::cout << "Caught signal " << sig 
              << ", shutting down server..." << std::endl;

    if (global_ircserv != NULL)
    {
        delete (global_ircserv);
    }
    std::exit(0);
}

int main(int argc, char **argv)
{
    parsing(argc, argv);

    Server *ircserv = new Server(argv[2]);
    if (!ircserv)
    {
        std::cerr << "Server allocation failed." << std::endl;
        return (1);
    }

    global_ircserv = ircserv;
    signal(SIGINT, handleShuttingDown);
    signal(SIGQUIT, handleShuttingDown);

    try
    {
        ircserv->RunServer(argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    delete (ircserv);

    return (0);
}
