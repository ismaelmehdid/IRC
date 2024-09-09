#include "../../include/server/Server.hpp"

Server::Server() : _socket(), _clients(), _channels(), _nbr_clients(0) {}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) { // closing every sockets of every clients
        delete it->second;
    }
}

/**
 * @brief Adds a client to the socket.
 * 
 * This function adds a client to the socket by storing it in the `_clients` map
 * using the client's file descriptor as the key. It also increments the `_nbr_clients`
 * counter.
 * 
 * @param client A pointer to the client object to be added.
 */
void    Server::add_client(Client *client) 
{
    _clients[client->get_fd()] = client;
    _nbr_clients++;
}

/**
 * @brief Removes a client from the Socket object.
 *
 * This function removes a client with the given file descriptor from the Socket object.
 * It deletes the client object and erases it from the internal map of clients.
 * The number of clients is decremented by one.
 *
 * @param fd The file descriptor of the client to be removed.
 */
void    Server::remove_client(int fd)
{
    delete _clients[fd];
    _clients.erase(fd);
    _nbr_clients--;
}