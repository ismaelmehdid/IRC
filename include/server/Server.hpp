#pragma once

# include "Socket.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../irc.hpp"
# include "../exception/ServerBindException.hpp"
# include "../exception/ServerCreationException.hpp"
# include "../exception/ServerListenException.hpp"
# include "../exception/PollException.hpp"

/**
 * @brief Main class representing the server.
 * 
 * This class is responsible for managing client connections, communication, and server events.
 * It handles incoming connections, client disconnections, and message processing.
 */
class Server
{
    private:
        unsigned int            _nbr_clients;      ///< Number of connected clients
        const std::string       _password;         ///< Server password for client authentication
        std::map<int, Client*>  _clients;          ///< Map of connected clients, key is client file descriptor
        Channel                 _channels;         ///< Channel for managing group communication
        std::vector<pollfd>     _fds;              ///< Poll file descriptors for client sockets
        pollfd                  _server_pollfd;    ///< Pollfd structure for the server socket
        int                     _poll_count;       ///< Number of poll events

        void                    serverLoop();

        void                    handlePollEvent(size_t i);
        void                    handleClientDisconnection(size_t i);
        void                    handleNewConnection();
        void                    handleClientMessage(size_t i);

        void                    addClient(Client *client);
        void                    removeClient(int fd);

    public:
        Server(const std::string &password);
        ~Server();

        void                    runServer(char **argv);
        const std::string       get_password() const;
        bool                    isNickNameTaken(const std::string &nickName);

        Socket                  _socket;           ///< Socket instance for network operations

};