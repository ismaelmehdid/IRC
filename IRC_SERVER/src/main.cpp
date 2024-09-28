#include "../include/server/Server.hpp"

Server  *global_ircserv = NULL;

int main(int argc, char **argv)
{
    validateArguments(argc, argv);
    
    try
    {
        global_ircserv = new Server(argv[2]);
        signal(SIGINT, handleShuttingDown);
        signal(SIGQUIT, handleShuttingDown);
        signal(SIGPIPE, SIG_IGN);
        global_ircserv->runServer(argv);
        delete (global_ircserv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        if (global_ircserv)
            delete (global_ircserv);
    }

    return (0);
}
