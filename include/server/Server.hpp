#pragma once

# include "Socket.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"

class Server
{
    public:

        Server(const std::string &password);
        ~Server();

        void                add_client(Client *client);
        void                remove_client(int fd);
        const std::string   get_password() const;

        Socket                  _socket;
        std::map<int, Client*>  _clients;
        Channel                 _channels;

    private:
        Server();

        unsigned int            _nbr_clients;
        const std::string       _password;
};