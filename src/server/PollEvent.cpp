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
void Server::handleClientMessage(size_t i)
{
    std::string message = _socket.receive(_fds[i].fd);
    if (message.empty())
    {
        handleClientDisconnection(i);
    }
    else
    {
        global_ircserv->_clients[_fds[i].fd]->execute_command(message);
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
void Server::handleNewConnection()
{
    int client_fd = _socket.accept();
    if (client_fd != -1)
    {
        pollfd client_pollfd;
        client_pollfd.fd = client_fd;
        client_pollfd.events = POLLIN;
        _fds.push_back(client_pollfd);

        Client* newClient = new Client(client_fd);
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
void Server::handleClientDisconnection(size_t i)
{
    std::cerr << "Client error or hangup on fd " << _fds[i].fd << std::endl;
    removeClient(_fds[i].fd);
    _fds.erase(_fds.begin() + i);
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
void Server::handlePollEvent(size_t i)
{
    if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
    {
        handleClientDisconnection(i);
    }
    else if (_fds[i].revents & POLLIN)
    {
        if (_fds[i].fd == _socket.get_fd())
        {
            handleNewConnection();
        }
        else
        {
            handleClientMessage(i);
        }
    }
}