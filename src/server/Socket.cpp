#include "../../include/server/Socket.hpp"
#include "../../include/irc.hpp"

Socket::Socket() : _sock_fd(-1), _backlog(5) {}

Socket::~Socket()
{
    close();
}

/**
 * @brief Creates a socket.
 * 
 * AF_INET      for the socket to use IPv4 address
 * SOCK_STREAM  for the socket to use TCP protocol
 * 
 * @return true if the socket is successfully created, false otherwise.
 */
bool Socket::create()
{
    this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_sock_fd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Binds the socket to a specific port.
 * 
 * AF_INET      indicating that the adress is an IPv4
 * INADDR_ANY   socket can accept connections from any available network interface
 * 
 * @param port The port number to bind the socket to.
 * @return True if the binding is successful, false otherwise.
 */
bool    Socket::bind(int port)
{
    sockaddr_in addr;

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); // convert port to big endian value (network communication standard) if needed

    if (::bind(this->_sock_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Bind failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Starts listening for incoming connections on the socket.
 * 
 * this->_backlog -> max clients in the queu
 * 
 * @return true if the listen operation was successful, false otherwise.
 */
bool    Socket::listen()
{
    if (::listen(this->_sock_fd, this->_backlog) == -1)
    {
        std::cerr << "Listen failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Accepts a new incoming connection on the socket.
 * 
 * @return The file descriptor of the accepted connection, or -1 if an error occurred.
 */
int Socket::accept()
{
    sockaddr_in client_addr; // client's network infos
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;
    
    client_fd = ::accept(this->_sock_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
        std::cerr << "Accept failed" << std::endl;

    return (client_fd);
}

/**
 * @brief Sends a message to the specified client socket.
 *
 * @param client_fd The file descriptor of the client socket.
 * @param message The message to be sent.
 * @return True if the message was sent successfully, false otherwise.
 */
bool    Socket::send(int client_fd, const std::string &message)
{
    if (::send(client_fd, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Send failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Receives data from a client socket.
 * 
 * This function receives data from the specified client socket and returns it as a string.
 * 
 * @param client_fd The file descriptor of the client socket.
 * @return The received data as a string.
 */
std::string Socket::receive(int client_fd)
{
    char    buffer[1024];
    int     bytes_received;

    std::memset(buffer, 0, sizeof(buffer));
    bytes_received = ::recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received == -1)
    {
        std::cerr << "Receive failed" << std::endl;
        return ("");
    }

    return (std::string(buffer));
}

/**
 * @brief Closes the socket.
 * 
 * This function closes the socket if it is open.
 * If the socket is already closed, this function does nothing.
 */
void    Socket::close()
{
    if (this->_sock_fd != -1)
    {
        ::close(this->_sock_fd);
        this->_sock_fd = -1;
    }
}

/**
 * @brief Closes the client socket.
 *
 * This function closes the client socket identified by the given file descriptor.
 * If the file descriptor is -1, no action is taken.
 *
 * @param client_fd The file descriptor of the client socket to be closed.
 */
void    Socket::close_client(int client_fd)
{
    if (client_fd != -1)
    {
        ::close(client_fd);
    }
}