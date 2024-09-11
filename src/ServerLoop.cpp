#include "../include/irc.hpp"

Server *global_ircserv = NULL; // is it allowed?

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

/**
 * @brief The main server loop that accepts client connections and handles communication.
 * 
 * This function continuously accepts client connections, receives messages from clients,
 * sends a response message, and closes the client connection. It runs in an infinite loop
 * until the program is terminated.
 * 
 * @param ircserv The Socket object representing the server socket.
 */
static void serverLoop(Server& ircserv)
{
    while (true)
    {
        int client_fd = ircserv._socket.accept();
        Client *newClient = perform_handshake(client_fd);

        if (newClient)
        {
            ircserv. add_client(newClient);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;

            std::string welcome_msg = ":server 001 client :Welcome to the IRC server!\r\n";
            if (!ircserv._socket.send(client_fd, welcome_msg))
            {
                std::cerr << "Error sending welcome message to client." << std::endl;
                ircserv.remove_client(client_fd);
                continue;
            }
        }
    }
}

/**
 * @brief           Starts the server and listens for incoming connections on the specified port.
 * 
 * @param ircserv   The Socket object representing the server.
 * @param argv      The command line arguments passed to the program.
 * @return          int Returns 0 if the server started successfully, otherwise returns 1.
 */
int start_server(Server *ircserv, char **argv)
{

    global_ircserv = ircserv;

    signal(SIGINT, handleShuttingDown);
    signal(SIGQUIT, handleShuttingDown);

    if (!ircserv->_socket.create()) return 1;
    if (!ircserv->_socket.bind(std::strtol(argv[1], NULL, 10))) return 1;
    if (!ircserv->_socket.listen()) return 1;

    std::cout << "Waiting for connections on port " << argv[1] << "..." << std::endl;
    serverLoop(*ircserv);
    delete ircserv;
    
    return 0;
}