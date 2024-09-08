#include "../../include/irc.hpp"

Socket *global_ircserv = NULL;

static void handle_sigint(int sig)
{
    std::cout << "Caught signal " << sig << ", shutting down server..." << std::endl;
    if (global_ircserv != NULL) {
        delete global_ircserv;
    }
    exit(0);
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
static void serverLoop(Socket& ircserv)
{
    while (true)
    {
        int client_fd = ircserv.accept();
        if (client_fd == -1)
            continue;

        Client *newClient = new Client(client_fd, new OperatorRole());
        if (newClient)
        {
            ircserv.add_client(newClient);
            std::cout << "Client connected!" << std::endl;

            std::string welcome_msg = ":server 001 client :Welcome to the IRC server!\r\n";
            if (!ircserv.send(client_fd, welcome_msg)) {
                std::cerr << "Error sending welcome message to client." << std::endl;
                ircserv.remove_client(client_fd);
                continue;
            }

            std::string received_message = ircserv.receive(client_fd);
            if (received_message.empty()) {
                std::cerr << "Error receiving message or connection closed by client." << std::endl;
                ircserv.remove_client(client_fd);
                continue;
            }
            std::cout << "Received: " << received_message << std::endl;
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
int start_server(Socket *ircserv, char **argv)
{

    global_ircserv = ircserv;

    signal(SIGINT, handle_sigint);

    if (!ircserv->create()) return 1;
    if (!ircserv->bind(std::strtol(argv[1], NULL, 10))) return 1;
    if (!ircserv->listen()) return 1;

    std::cout << "Waiting for connections on port " << argv[1] << "..." << std::endl;
    serverLoop(*ircserv);
    delete ircserv;
    
    return 0;
}