#pragma once

#include "../client/Client.hpp"

class Socket
{
    private:
        int                     _fd;
        int                     _backlog; // max clients in queu

    public:
        Socket();
        ~Socket();

        bool        bind(int port);
        bool        create();
        bool        listen();
        Client*     accept();
        bool        send(int client_fd, const std::string &message);
        std::string receive(int client_fd, bool &tempErr);
        int         get_fd() const;

};