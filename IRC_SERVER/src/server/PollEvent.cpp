#include "../../include/server/Server.hpp"

/**
 * @brief Handles incoming messages from a client.
 *
 * This function processes messages received from a client identified by the file descriptor
 * at the given index in the _fds array. It retrieves the client from the _clients map, receives
 * the message from the socket, and processes it. If the message is empty, it handles client 
 * disconnection. Otherwise, it appends the message to the client's buffer and processes commands 
 * separated by "\r\n". If command execution fails or the client disconnects, it handles the 
 * disconnection.
 *
 * @param i The index of the file descriptor in the _fds array.
 */
void    Server::handleClientMessage(size_t i)
{
    std::map<int, Client*>::iterator it = _clients.find(_fds[i].fd);
    if (it == _clients.end())
        return;
    
    Client* client = it->second;
    bool        tempErr = false;
    std::string message = this->_socket.receive(_fds[i].fd, tempErr);
    
    if (tempErr)
        return;

    if (message.empty())
    {
        handleClientDisconnection(i);
        return;
    }

    client->getBuffer() += message;

    size_t      pos;
    while ((pos = client->getBuffer().find("\r\n")) != std::string::npos) 
    {
        std::string command = client->getBuffer().substr(0, pos + 2);
        client->getBuffer().erase(0, pos + 2);
        
        if (!this->executeCommand(client, command))
        {
            handleClientDisconnection(i);
        }
        
        if (_clients.find(_fds[i].fd) == _clients.end())
            return ;
    }
    if (pos == std::string::npos)
    {
        std::cout << RED << "Partial command received from client on fd " << _fds[i].fd << RESET << std::endl;
    }
}

/**
 * @brief Handles new incoming connections to the server.
 * 
 * This function checks if the server has reached its maximum client capacity.
 * If the server is full, it rejects the new connection and sends an error message
 * to the client. If there is room for more clients, it accepts the new connection,
 * sets up the necessary polling structures, and adds the client to the server's
 * client list.
 * 
 * @note If the server is full, the rejected client is properly deleted to avoid memory leaks.
 */
void    Server::handleNewConnection()
{
    if (this->_nbr_clients >= MAX_CLIENTS)
    {
        Client* rejectedClient = this->_socket.accept();
        
        if (rejectedClient)
        {
            std::string rejectMsg = "ERROR :Server is full, try again later.\r\n";
            this->_socket.Send(rejectedClient->get_fd(), rejectMsg);
            std::cerr << RED << "Connection rejected: server is full (fd " << rejectedClient->get_fd() << ")" << RESET << std::endl;
            delete (rejectedClient);
        }
        return ;
    }

    Client* newClient = this->_socket.accept();

    if (newClient)
    {
        pollfd  client_pollfd;

        std::cout << MAGENTA << "New connection on fd " << newClient->get_fd() << RESET << std::endl;
        
        client_pollfd.fd = newClient->get_fd();
        client_pollfd.events = POLLIN;
        client_pollfd.revents = 0;
        _fds.push_back(client_pollfd);
        addClient(newClient);
    }
}

/**
 * @brief Handles the disconnection of a client.
 *
 * This function is responsible for managing the disconnection of a client
 * identified by the file descriptor at the given index in the _fds vector.
 * It retrieves the client associated with the file descriptor, logs an error
 * message if the client is found, removes the client, and updates the _fds
 * vector and _poll_count accordingly.
 *
 * @param i The index of the file descriptor in the _fds vector.
 */
void    Server::handleClientDisconnection(size_t i)
{
    int     fd = this->_fds[i].fd;
    Client* client = this->_clients[fd];

    if (client)
    {
        std::cerr << RED << "Client error on fd " << fd << RESET << std::endl;
        removeClient(client, "disconnected\r\n");
        return ;
    }
    _fds.erase(_fds.begin() + i);
}

/**
 * @brief Handles poll events for the server.
 *
 * This function processes different types of poll events for the server. It checks the revents field
 * of the pollfd structure to determine the type of event that occurred and takes appropriate actions.
 *
 * @param i The index of the pollfd structure in the _fds vector.
 *
 * - If the event is POLLHUP, POLLOUT, or POLLNVAL, it handles client disconnection.
 * - If the event is POLLIN, it checks if the event is on the server's socket or a client's socket:
 *   - If it's on the server's socket, it handles a new connection.
 *   - If it's on a client's socket, it handles a client message.
 */
void    Server::handlePollEvent(size_t i)
{
    if (this->_fds[i].revents & (POLLHUP | POLLOUT | POLLNVAL))
    {
        handleClientDisconnection(i);
    }
    else if (_fds[i].revents & POLLIN)
    {
        if (this->_fds[i].fd == this->_socket.get_fd()) // there is smth to read on the server's socket
        {
            handleNewConnection();
        }
        else
        {
            handleClientMessage(i);
        }
    }
}
