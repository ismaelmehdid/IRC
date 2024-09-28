#pragma once

#include "../irc.hpp"

//===----------------------------------------------------------------------===//

class Client
{
    private:
        Client (const Client& other);
        Client& operator=(const Client& other);

        int             _fd;
        std::string     _hostMask;
        std::string     _nickName;
        std::string     _userName;
        std::string     _fullName;
        std::string     _buffer;

    public:
        Client(int fd, char *host);

        ~Client();

//=== Getters -------------------------------------------------------------===//

        std::string&    getNickName();
        std::string&    getUserName();
        std::string&    getFullName();
        std::string&    getHostMask();
        std::string     getPrefix() const;
        std::string&    getBuffer();
        int             get_fd() const;
        bool            is_authenticated();

//=== Setters -------------------------------------------------------------===//
        void            setNickName(const std::string &nickName);
        void            setUserName(const std::string &username);
        void            setFullName(const std::string &fullname);
           
//=== Auth ----------------------------------------------------------------===//
        bool        _has_set_password;
};

//===----------------------------------------------------------------------===//