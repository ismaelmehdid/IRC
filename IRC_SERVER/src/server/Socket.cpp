#include "../../include/server/Socket.hpp"
#include "../../include/irc.hpp"

Socket::Socket() : _fd(-1), _backlog(MAX_CLIENTS) {}

Socket::~Socket() 
{
    close(this->_fd);
}

Socket::Socket(const Socket &socket) : _fd(socket._fd), _backlog(socket._backlog) {}

Socket &Socket::operator=(const Socket &socket)
{
    if (this != &socket) {
        _fd = socket._fd;
        _backlog = socket._backlog;
    }
    return *this;
}


/**
 * @brief Creates a socket and sets it to non-blocking mode.
 * 
 * For Linux:
 * - AF_INET: Uses IPv4 addresses.
 * - SOCK_STREAM: Uses the TCP protocol.
 * - SOCK_NONBLOCK: Sets the socket to non-blocking mode at creation.
 * 
 * For macOS:
 * - AF_INET: Uses IPv4 addresses.
 * - SOCK_STREAM: Uses the TCP protocol.
 * - fcntl is used to set the socket to non-blocking mode after creation.
 * 
 * @return true if the socket is successfully created and set to non-blocking mode, false otherwise.
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
    // allows to connect no port while its on TIME_WAIT status
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
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
 * this->_backlog -> max clients in the queu
 * 
 * @return true if the listen operation was successful, false otherwise.
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
 * @brief Accepts a new incoming connection on the socket.
 * 
 * @return The file descriptor of the accepted connection, or -1 if an error occurred.
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
 * @brief Sends a message to the specified client socket.
 *
 * @param client_fd The file descriptor of the client socket.
 * @param message The message to be sent.
 * @return True if the message was sent successfully, false otherwise.
 */
bool    Socket::send(int client_fd, const std::string &message)
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
 * This function receives data from the specified client socket and returns it as a string.
 * 
 * @param client_fd The file descriptor of the client socket.
 * @return The received data as a string.
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
