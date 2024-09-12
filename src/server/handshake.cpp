#include "../../include/irc.hpp"
#include "../../include/server/Server.hpp"

// static void print_received(const std::string &received)
// {
//     size_t                      start = 0;
//     size_t                      end;

//     while ((end = received.find("\r\n", start)) != std::string::npos) {
//         std::cout << received.substr(start, end - start) << std::endl;
//         start = end + 2;
//     }
//     if (start < received.size()) {
//         std::cout << received.substr(start) << std::endl;
//     }
// }

Client *perform_handshake(int client_fd)
{
    if (client_fd == -1) {
        return NULL;
    }

    std::string server_password = global_ircserv->get_password();

    std::string connection_msg = global_ircserv->_socket.receive(client_fd);
    std::cout << "size of the message:" << connection_msg.size() << std::endl;
    if (connection_msg.empty())
    {
        std::cerr << "Error receiving message or connection closed by client." << std::endl;
        return NULL;
    }

    for (size_t i = 0; i != connection_msg.size(); i++) {
        if (connection_msg[i] != '\r') {
            std::cout << connection_msg[i];
        }
    }

    // Client *newClient = new Client(nick, username, realname, client_fd, NULL);
    // if (!newClient)
    //     ::close(client_fd);

    return NULL;
}
