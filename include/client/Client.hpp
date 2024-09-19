#pragma once

#include "../irc.hpp"

# include <string>
# include <map>

class ARole;

class Client
{
    private:
        int                                     _fd;
        std::string                             _hostMask;
        std::string                             _nickName;
        std::string                             _userName;
        std::string                             _fullName;

    public:
        Client(int fd, char *host);
        Client (const Client& other);
        Client& operator=(const Client& other);
        ~Client();

//----------Getters
        std::string getNickName() const;
        std::string getUserName() const;
        std::string getFullName() const;
        std::string getHostMask() const;
        std::string getPrefix() const;
        int         get_fd() const;
        bool        is_authenticated();

//----------Setters
        void        setNickName(const std::string &nickName);
        void        setUserName(const std::string &username);
        void        setFullName(const std::string &fullname);
           
//----------Auth
        bool        _has_set_password;
};