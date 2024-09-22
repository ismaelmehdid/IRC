#include "../../include/server/Server.hpp"

/**
 * @brief Handles the message received from a client.
 * 
 * This function processes the message received from the client identified by the given pollfd index.
 * If the message is empty, it handles the client's disconnection. Otherwise, it prints and sends
 * a confirmation message back to the client.
 * 
 * @param i Index of the client in the _fds array.
 */
void    Server::handleClientMessage(size_t i)
{
    Client*     client  = _clients[_fds[i].fd];
    std::string message = this->_socket.receive(_fds[i].fd);
    
    if (message.empty()) 
    {
        handleClientDisconnection(i);
    } 
    else 
    {
        client->getBuffer() += message;

        size_t pos;
        while ((pos = client->getBuffer().find('\n')) != std::string::npos) 
        {
            std::string command = client->getBuffer().substr(0, pos + 2);
            std::cout << "Received message: " << command << std::endl;
            client->getBuffer().erase(0, pos + 2);
            this->executeCommand(client, command);
        }
    }
}

/**
 * Handles a new client connection.
 * 
 * This function accepts a new client connection, creates a pollfd structure for the client,
 * adds it to the list of file descriptors to be polled, creates a new Client object for the client,
 * adds the client to the server's list of clients, sends a welcome message to the client,
 * and prints a message indicating that a client has connected.
 */
void    Server::handleNewConnection()
{
    Client* newClient = this->_socket.accept();

    if (newClient)
    {
        pollfd  client_pollfd;

        client_pollfd.fd = newClient->get_fd();
        client_pollfd.events = POLLIN;
        client_pollfd.revents = 0;
        _fds.push_back(client_pollfd);
        addClient(newClient);
    }
}

/**
 * @brief Handles a client's disconnection from the server.
 * 
 * This function handles the disconnection of the client at the given index in the pollfd array.
 * It closes the client’s socket, removes the client from the server, and erases the pollfd entry.
 * 
 * @param i Index of the client in the _fds array.
 */
void    Server::handleClientDisconnection(size_t i)
{
    int     fd      = this->_fds[i].fd;
    Client* client  = this->_clients[fd];

    if (client)
    {
        std::cerr << RED << "Client error or hangup on fd " << fd << RESET << std::endl;

        removeClient(client, "disconnected");
    }
    else
    {
        std::cerr << RED << "Error: Client not found for fd " << fd << RESET <<std::endl;
    }
}

/**
 * @brief Handles poll events for a specific file descriptor.
 * 
 * This function processes the poll events for the client or server socket identified by the index in the pollfd array.
 * It checks if there are any errors or hangups, in which case it handles the disconnection. If the event is a 
 * readable event (POLLIN), it either handles a new connection (if it's the server socket) or a client message 
 * (if it's a client socket).
 * 
 * @param i Index of the client in the _fds array.
 */
void    Server::handlePollEvent(size_t i)
{
    if (this->_fds[i].revents & (POLLHUP | POLLOUT))
    {
        handleClientDisconnection(i);
    }
    else if (_fds[i].revents & POLLIN)
    {
        if (this->_fds[i].fd == this->_socket.get_fd()) // there is smth to read on the server socket (which mean a new connection)
        {
            handleNewConnection();
        }
        else
        {
            handleClientMessage(i);
        }
    }
}