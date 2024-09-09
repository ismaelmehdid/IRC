#pragma once

# include "Socket.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"

class Server
{
    public:
        Server();
        ~Server();

        void    add_client(Client *client);
        void    remove_client(int fd);

        Socket                  _socket;
        std::map<int, Client*>  _clients;
        Channel                 _channels;

    private:
        unsigned int            _nbr_clients;
};