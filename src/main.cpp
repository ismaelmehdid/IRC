#include "../include/irc.hpp"

void    serverLoop(Socket& ircserv)
{
    while (true)
    {
        int client_fd = ircserv.accept();
        if (client_fd == -1)
        {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }
        std::cout << "Client connected!" << std::endl;
        std::string received_message = ircserv.receive(client_fd);
        std::cout << "Received: " << received_message << std::endl;
        ircserv.send(client_fd, "Hello from server!");
        ircserv.close_client(client_fd);
    }
}

int main(int argc, char **argv)
{
    if (argc != ARGUMENTS_REQUIRED)
    {
        display_error_message(ERR_BAD_ARGUMENTS);
        irc_exit(EXIT_FAILURE);
    }

    if (!checkPort((argv[1])))
    {
        display_error_message(ERR_BAD_PORT);
        irc_exit(EXIT_FAILURE);
    }

    Socket ircserv;

    if (!ircserv.create()) return 1;
    if (!ircserv.bind(std::strtol(argv[1], NULL, 10))) return 1;
    if (!ircserv.listen()) return 1;

    std::cout << "Waiting for connections on port " << argv[1] << "..." << std::endl;
    serverLoop(ircserv);
    ircserv.close();
    return 0;
}
