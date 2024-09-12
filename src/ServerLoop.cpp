#include "../include/irc.hpp"
#include "../include/server/Server.hpp"

Server *global_ircserv = NULL; // allowed

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
    std::vector<pollfd> fds;
    pollfd              server_pollfd;

    server_pollfd.fd = ircserv._socket.get_fd();
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    while (true)
    {
        int poll_count = poll(fds.data(), fds.size(), -1);

        if (poll_count == -1)
        {
            std::cerr << "Poll failed" << std::endl;
            break ;
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                std::cerr << "Client error or hangup on fd " << fds[i].fd << std::endl;
                close(fds[i].fd);
                ircserv.remove_client(fds[i].fd);
                fds.erase(fds.begin() + i);
                i--;
                continue;
            }
            else if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == ircserv._socket.get_fd())
                {
                    std::cout << GREEN << "im new client" << RESET << std::endl << std::endl;
                    int client_fd = ircserv._socket.accept();
                    if (client_fd != -1)
                    {
                        pollfd  client_pollfd;
                        client_pollfd.fd = client_fd;
                        client_pollfd.events = POLLIN;
                        fds.push_back(client_pollfd);
                        Client *newClient = perform_handshake(client_fd);
                        if (newClient)
                        {
                            ircserv.add_client(newClient);
                            std::cout << GREEN << "Client connected!" << RESET << std::endl;

                            std::string welcome_msg = ":server 001 client :Welcome to the IRC server!\r\n";
                            ircserv._socket.send(client_fd, welcome_msg);
                        }
                    }
                }
                else
                {
                    std::cout << MAGENTA << "im message from client" << RESET << std::endl << std::endl;
                    std::string message = ircserv._socket.receive(fds[i].fd);
                    if (message.empty())
                    {
                        std::cerr << "Client disconnected" << std::endl;
                        close(fds[i].fd);
                        ircserv.remove_client(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                    else
                    {
                        std::cout << message << std::endl;
                        ircserv._socket.send(fds[i].fd, "Message received\n");
                    }
                }
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