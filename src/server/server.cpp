#include "../../include/irc.hpp"

/**
 * @brief The main server loop that accepts client connections and handles communication.
 * 
 * This function continuously accepts client connections, receives messages from clients,
 * sends a response message, and closes the client connection. It runs in an infinite loop
 * until the program is terminated.
 * 
 * @param ircserv The Socket object representing the server socket.
 */
static void serverLoop(Socket& ircserv)
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

/**
 * @brief           Starts the server and listens for incoming connections on the specified port.
 * 
 * @param ircserv   The Socket object representing the server.
 * @param argv      The command line arguments passed to the program.
 * @return          int Returns 0 if the server started successfully, otherwise returns 1.
 */
int start_server(Socket &ircserv, char **argv)
{
    if (!ircserv.create()) return 1;
    if (!ircserv.bind(std::strtol(argv[1], NULL, 10))) return 1;
    if (!ircserv.listen()) return 1;

    std::cout << "Waiting for connections on port " << argv[1] << "..." << std::endl;
    serverLoop(ircserv);
    ircserv.close();
    
    return 0;
}