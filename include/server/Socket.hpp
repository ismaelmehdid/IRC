#pragma once

#include "../client/Client.hpp"
#include <map>

class Socket
{
    private:
        int                     _fd;
        int                     _backlog; // max clients in queu
        unsigned int            _nbr_clients;
        std::map<int, Client*>   _clients;

    public:
        Socket();
        // Socket (const Socket& other);
        // Socket& operator=(const Socket& other);
        ~Socket();

        bool        bind(int port);
        bool        create();
        bool        listen();
        int         accept();
        bool        send(int client_fd, const std::string &message);
        std::string receive(int client_fd);

        void        add_client(Client *client);
        void        remove_client(int fd);
};