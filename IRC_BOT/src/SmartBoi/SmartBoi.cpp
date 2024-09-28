#include "../../include/SmartBoi/SmartBoi.hpp"
#include "../../include/smartboi.hpp"

SmartBoi::SmartBoi() : _irc_socket_fd(-1), _api_socket_fd(-1), _api_key("") {}

SmartBoi::~SmartBoi()
{
    close(_irc_socket_fd);
}

SmartBoi::SmartBoi(const SmartBoi &toCopy) : _irc_socket_fd(toCopy._irc_socket_fd), _api_socket_fd(toCopy._api_socket_fd), _api_key(toCopy._api_key) {}

SmartBoi &SmartBoi::operator=(const SmartBoi &toCopy)
{
    if (this != &toCopy) {
        _irc_socket_fd = toCopy._irc_socket_fd;
        _api_socket_fd = toCopy._api_socket_fd;
        _api_key = toCopy._api_key;
    }
    return *this;
}

/**
 * @brief Starts the SmartBoi bot by connecting to the IRC server and entering the main loop.
 * 
 * This function initializes the bot by retrieving the API key, connecting to the specified IRC server,
 * and then entering the main event loop to handle incoming messages and events.
 * 
 * @param server_ip The IP address of the IRC server to connect to.
 * @param server_port The port number of the IRC server to connect to.
 * @param server_password The password for the IRC server, if required.
 */
void SmartBoi::start(const std::string &server_ip, const std::string &server_port, const std::string &server_password)
{
    _api_key = get_API_key();
    this->connect_to_irc_server(server_ip, server_port, server_password);
    this->loop();
}

/**
 * @brief Handles the response for a given IRC request.
 *
 * This function processes the incoming IRC request, parses the commands,
 * and if the command is "PRIVMSG", it calls a weather API based on the message content.
 * The response from the weather API is then parsed and sent back to the IRC server.
 *
 * @param request The incoming IRC request as a string.
 */
void SmartBoi::handle_response(const std::string &request)
{
    std::vector<t_IRCCommand>   parsed_commands = parseRequests(request);

    for (size_t i = 0; i < parsed_commands.size(); i++) {
        if (parsed_commands[i].command == "PRIVMSG") {

            const std::string &from = parsed_commands[i].prefix;

            const std::string &message = parsed_commands[i].trailing;
            std::string api_response = call_weather_api(message);
            std::string parsed_api_response = parse_weather_api_response(api_response, message);

            const std::string to_send = "PRIVMSG " + from + " :" + parsed_api_response + "\r\n";

            if (send(_irc_socket_fd, to_send.c_str(), to_send.size(), 0) < 0) {
                std::cerr << "Error while sending sending a response to the server." << std::endl;
            }

        }
    }
}

/**
 * @brief Continuously listens for incoming messages from the IRC server and processes them.
 * 
 * This function runs an infinite loop that waits for data from the IRC server. It uses the 
 * `recv` function to receive data into a buffer. If an error occurs during reception, or if 
 * the server closes the connection, it throws a runtime exception. Otherwise, it processes 
 * the received data using the `handle_response` function.
 * 
 * @throws std::runtime_error If an error occurs while receiving data or if the server closes the connection.
 */
void SmartBoi::loop()
{
    char buffer[4096];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(_irc_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            throw std::runtime_error("Error while receiving data from the server.");
        } else if (bytesReceived == 0) {
            throw std::runtime_error("The IRC server closed the connection.");
        } else {
            handle_response(buffer);
        }
    }
}

/**
 * @brief Connects to an IRC server using the provided server IP, port, and password.
 *
 * This function creates a socket, sets up the server address structure, and attempts
 * to connect to the specified IRC server. Upon successful connection, it sends the
 * authentication credentials (password, user, and nickname) to the server.
 *
 * @param server_ip The IP address of the IRC server to connect to.
 * @param server_port The port number of the IRC server to connect to.
 * @param password The password for authenticating with the IRC server.
 *
 * @throws std::runtime_error If there is an error creating the socket, converting the
 *                            server port, connecting to the server, or sending the
 *                            authentication credentials.
 */
void SmartBoi::connect_to_irc_server(const std::string &server_ip, const std::string &server_port, const std::string &password)
{
    _irc_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_irc_socket_fd < 0) {
        throw std::runtime_error("Error while creating the socket to connect with the IRC server.");
    }

    struct sockaddr_in serverAddr;

    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    std::stringstream iss(server_port);
    long converted_port;
    iss >> converted_port;
    if (iss.fail() || !iss.eof()) {
        throw std::runtime_error("Error while converting the server port to long value.");
    }

    serverAddr.sin_port = htons(converted_port);

    if (connect(_irc_socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Error while connecting to the IRC server.");
    }

    std::string name = "smartboi";
    std::string authMessage = "PASS " + password + "\r\n";
    authMessage += "USER " + name + " 0 * :" + name + "\r\n";
    authMessage += "NICK " + name + "\r\n";

    if (send(_irc_socket_fd, authMessage.c_str(), authMessage.size(), 0) < 0) {
        throw std::runtime_error("Error while sending auth credentials to the server.");
    }
}

int SmartBoi::get_irc_socket_fd() const
{
    return _irc_socket_fd;
}
