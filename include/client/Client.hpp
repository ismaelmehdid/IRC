#pragma once

# include "OperatorRole.hpp"
# include "RegularRole.hpp"

class IRole;

class Client
{
    public:
        Client(const std::string& nickName, const std::string& userName, const std::string& fullName, int fd, IRole* role);
        Client (const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        int         get_fd() const;
        void        executeKick();
        void        executeInvite();
        void        executeTopic();
        void        executeMode(char arg);
        void        setRole(IRole* newRole);

    private:
        Client();

        std::string _nickName; // the server must check uniqueness
        std::string _userName;
        std::string _fullName;
        int         _fd;
        IRole*      _role;
        bool        _isAuthenticated;
};