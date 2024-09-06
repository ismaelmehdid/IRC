#pragma once

#include "irc.hpp"

class Socket
{
    private:
        int         _sock_fd;
        int         _backlog;

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
        void        close();
        void        close_client(int client_fd);
};