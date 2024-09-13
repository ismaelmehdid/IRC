#pragma once

# include "Socket.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../irc.hpp"
# include "../exception/ServerBindException.hpp"
# include "../exception/ServerCreationException.hpp"
# include "../exception/ServerListenException.hpp"
# include "../exception/PollException.hpp"

class Server
{
    private:
        unsigned int            _nbr_clients;
        const std::string       _password;
        Socket                  _socket;
        std::map<int, Client*>  _clients;
        Channel                 _channels;
        std::vector<pollfd>     _fds;
        pollfd                  _server_pollfd;
        int                     _poll_count;
    
        void                    ServerLoop();

        void                    handlePollEvent(size_t i);
        void                    handleClientDisconnection(size_t i);
        void                    handleNewConnection();
        void                    handleClientMessage(size_t i);

        void                    addClient(Client *client);
        void                    removeClient(int fd);
        Client                  *perform_handshake(int client_fd);

    public:
        Server(const std::string &password);
        ~Server();

        void                    RunServer(char **argv);
        const std::string       get_password() const;
};