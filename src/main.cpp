#include "../include/server/Server.hpp"

Server  *global_ircserv = NULL;

int main(int argc, char **argv)
{
    validate_provided_args(argc, argv);

    global_ircserv = new Server(argv[2]);
    if (!global_ircserv)
    {
        std::cerr << "Server allocation failed." << std::endl;
        return (1);
    }

    signal(SIGINT, handleShuttingDown);
    signal(SIGQUIT, handleShuttingDown);

    try {
        global_ircserv->runServer(argv);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    delete (global_ircserv);

    return (0);
}
