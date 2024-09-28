#include "../../include/SmartBoi/SmartBoi.hpp"
#include "../../include/smartboi.hpp"

/**
 * @brief Calls the weather API with the given input and retrieves the response.
 *
 * This function connects to the weather API, sends a GET request with the specified input,
 * and retrieves the response. The response is returned as a string.
 *
 * @param input The input string to be used in the API request (e.g., city name).
 * @return A string containing the response from the weather API.
 */
std::string SmartBoi::call_weather_api(const std::string &input)
{
    this->connect_to_weather_api();

    std::string host = WEATHER_API_HOST;
    std::string path = "/data/2.5/weather?q=" + input + "&appid=" + _api_key + "&units=metric";

    // GET request conforming with the HTTP protocol
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

    // Sending the request
    if (send(_api_socket_fd, request.c_str(), request.length(), 0) < 0) {
        std::cerr << "Failed to send the request to the API." << std::endl;
        close(_api_socket_fd);
        return "";
    }

    // Get the request
    char buffer[4096];
    std::string response;
    
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(_api_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            break; // End of the request
        }
        response.append(buffer, bytesRead);
    }
    close(_api_socket_fd);
    return response;
}

/**
 * @brief Connects to the weather API server.
 *
 * This function establishes a socket connection to the weather API server.
 * It performs the following steps:
 * 1. Creates a socket for communication.
 * 2. Resolves the DNS of the API server.
 * 3. Sets up the server address structure.
 * 4. Connects to the API server using the created socket.
 *
 * @throws std::runtime_error if there is an error creating the socket,
 *                            resolving the DNS, or connecting to the server.
 */
void SmartBoi::connect_to_weather_api()
{
    _api_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_api_socket_fd < 0) {
        throw std::runtime_error("Error while creating the socket to connect with the API.");
    }

    struct hostent *server = gethostbyname(WEATHER_API_HOST);
    if (server == NULL) {
        close (_api_socket_fd);
        throw std::runtime_error("Error resolving the DNS of the API server.");
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(HTTP_PORT); // HTTP Port
    std::memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length); // Get the ip address with the domain name

    // Connecting to the API server
    if (connect(_api_socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close (_api_socket_fd);
        throw std::runtime_error("Error while connecting to the API server.");
    }
}