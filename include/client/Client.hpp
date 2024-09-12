#pragma once

# include "OperatorRole.hpp"
# include "RegularRole.hpp"

# include <string>
# include <map>

class IRole;

class Client
{
    public:
        Client(const std::string& nickName, const std::string& userName, const std::string& fullName, int fd, IRole* role);
        Client (const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        int         get_fd() const;
        void        execute_command(const std::string &message);

    private:
        Client();

        typedef void (Client::*CommandFunction)(const t_IRCCommand &);
        void        initializeCommandMap();

        void        executeKick     (const t_IRCCommand &);
        void        executeInvite   (const t_IRCCommand &);
        void        executeTopic    (const t_IRCCommand &);
        void        executeMode     (const t_IRCCommand &);
        void        executePass     (const t_IRCCommand &);
        void        executeNick     (const t_IRCCommand &);
        void        executeUser     (const t_IRCCommand &);

        void        setRole         (IRole* newRole);

        std::string                             _nickName; // the server must check uniqueness
        std::string                             _userName;
        std::string                             _fullName;
        int                                     _fd;
        IRole*                                  _role;
        bool                                    _isAuthenticated;
        std::map<std::string, CommandFunction>  _commandMap; // map associating every command string to the proper function ptr
};