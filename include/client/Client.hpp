#pragma once

# include "OperatorRole.hpp"
# include "RegularRole.hpp"

class IRole;

class Client
{
    public:
        Client();
        Client(const std::string& nickName, int fd, IRole* role);
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
        std::string _nickName; // the server must check uniqueness
        int         _fd;
        IRole*      _role;
};