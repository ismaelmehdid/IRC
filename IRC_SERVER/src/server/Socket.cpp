#include "../../include/server/Server.hpp"

Socket::Socket() : _fd(-1), _backlog(MAX_CLIENTS) {}

Socket::~Socket() 
{
    if (this->_fd != -1)
        close(this->_fd);
}

Socket::Socket(const Socket &socket) : _fd(socket._fd), _backlog(socket._backlog) {}

Socket &Socket::operator=(const Socket &socket)
{
    if (this != &socket)
    {
        this->_fd = socket._fd;
        this->_backlog = socket._backlog;
    }

    return (*this);
}

/**
 * @brief Creates a socket and sets it to non-blocking mode.
 *
 * This function creates a socket using the appropriate system calls for the
 * current operating system. On Linux, it creates a non-blocking socket directly.
 * On macOS, it creates a socket and then sets it to non-blocking mode using fcntl.
 * 
 * @return true if the socket was successfully created and configured, false otherwise.
 */
bool    Socket::create()
{
#ifdef __linux__
    this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
#elif __APPLE__
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (this->_fd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return (false);
    }

    if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Failed to set socket to non-blocking mode" << std::endl;
        return (false);
    }
#endif

    if (this->_fd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return (false);
    }
    
    int opt = 1;
    // allows to connect to port while its on TIME_WAIT status
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        return (false);
    }

    return (true);
}

/**
 * @brief Binds the socket to the specified port.
 *
 * This function sets up the socket address structure with the given port and binds the socket
 * to it. The address is set to accept connections from any IP address (INADDR_ANY).
 *
 * @param port The port number to bind the socket to.
 * @return true if the binding is successful, false otherwise.
 */
bool    Socket::bind(int port)
{
    sockaddr_in addr;

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); // convert port to big endian value (network communication standard) if needed

    if (::bind(this->_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Bind failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Starts listening for incoming connections on the socket.
 * 
 * This function sets the socket to listen for incoming connection requests.
 * It uses the file descriptor and backlog value stored in the Socket object.
 * 
 * @return true if the socket successfully starts listening, false otherwise.
 */
bool    Socket::listen()
{
    if (::listen(this->_fd, this->_backlog) == -1)
    {
        std::cerr << "Listen failed" << std::endl;
        return (false);
    }
    return (true);
}

/**
 * @brief Accepts a new client connection.
 *
 * This function waits for a new client connection on the socket and accepts it.
 * It retrieves the client's network information, including the IP address, and
 * creates a new Client object with the client's file descriptor and IP address.
 *
 * @return A pointer to a new Client object if the connection is successfully accepted,
 *         or NULL if the accept operation fails.
 */
Client* Socket::accept()
{
    sockaddr_in client_addr; // client's network infos
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;
    char	    host[INET_ADDRSTRLEN];
    
    client_fd = ::accept(this->_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
    {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return (NULL);
    }
    inet_ntop(AF_INET, &client_addr.sin_addr, host, INET_ADDRSTRLEN); // convert IP address from int to string

    return (new Client(client_fd, host));
}

/**
 * @brief Sends a message to a client socket.
 * 
 * This function attempts to send the entire message to the specified client socket.
 * It handles partial sends and retries until the entire message is sent or an error occurs.
 * 
 * @param client_fd The file descriptor of the client socket.
 * @param message The message to be sent to the client.
 * @return true if the message was successfully sent or if the send operation would block.
 * @return false if an error occurred during the send operation.
 */
bool Socket::Send(int client_fd, const std::string &message)
{
    size_t      total_sent     = 0;
    size_t      message_length = message.size();
    const char  *message_cstr  = message.c_str();

    while (total_sent < message_length)
    {
        ssize_t sent = ::send(client_fd, message_cstr + total_sent, message_length - total_sent, 0);
        if (sent == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return (true);
            }
            else if (errno == EPIPE)
            {
                std::cerr << "Client disconnected (SIGPIPE): " << strerror(errno) << std::endl;
                return (false);
            }
            else
            {
                std::cerr << "Send failed: " << strerror(errno) << std::endl;
                return (false);
            }
        }
        total_sent += sent;
    }
    return (true);
}

/**
 * @brief Receives data from a client socket.
 *
 * This function reads data from the specified client socket file descriptor
 * and appends it to a result string. It continues reading until there is no
 * more data to read or an error occurs.
 *
 * @param client_fd The file descriptor of the client socket.
 * @param tempErr A reference to a boolean that will be set to true if a 
 *        temporary error occurs (EAGAIN or EWOULDBLOCK), indicating that 
 *        the operation should be tried again later. Otherwise, it will be 
 *        set to false.
 * @return A string containing the data received from the client socket. If 
 *         an error occurs, an empty string is returned.
 */
std::string Socket::receive(int client_fd, bool& tempErr)
{
    char        buffer[512];
    std::string result;
    ssize_t     bytes_received;

    std::memset(buffer, 0, sizeof(buffer));

    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0)
    {
        result.append(buffer, bytes_received);
        if (bytes_received < static_cast<ssize_t>(sizeof(buffer)))
            break ;
        
        std::memset(buffer, 0, sizeof(buffer));
    }

    if (bytes_received == -1)
    {
        // meaning there is nothing else to read for the moment
        if (errno == EAGAIN || errno == EWOULDBLOCK) // temporary error, should try later
            tempErr = true;
        else
            std::cerr << "Receive failed: " << strerror(errno) << std::endl;
        return ("");
    }

    tempErr = false;
    return (result);
}

int Socket::get_fd() const
{
    return (this->_fd);
}
