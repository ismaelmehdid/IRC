#pragma once

#include "../client/Client.hpp"

//===----------------------------------------------------------------------===//

class Socket
{
    private:
        Socket(const Socket &socket);
        Socket &operator=(const Socket &socket);

        int                     _fd;
        int                     _backlog; // max clients in queu
        
    public:
        Socket();
        ~Socket();

        bool        bind(int port);
        bool        create();
        bool        listen();
        Client*     accept();
        bool        Send(int client_fd, const std::string &message);
        std::string receive(int client_fd, bool &tempErr);
        int         get_fd() const;

};

//===----------------------------------------------------------------------===//