#pragma once

# include "OperatorRole.hpp"
# include "RegularRole.hpp"
# include <string>
# include <map>

class ARole;

class Client
{
    private:
        typedef void (Client::*CommandFunction)(const t_IRCCommand &);
        void        initializeCommandMap();

        std::string                             _hostMask;
        std::string                             _nickName;
        std::string                             _userName;
        std::string                             _fullName;
        int                                     _fd;
        ARole*                                  _role;
        std::map<std::string, CommandFunction>  _commandMap;

//----------Commands
        void        executeKick     (const t_IRCCommand &);
        void        executeInvite   (const t_IRCCommand &);
        void        executeTopic    (const t_IRCCommand &);
        void        executeMode     (const t_IRCCommand &);
        void        executeCap      (const t_IRCCommand &);
        void        executePass     (const t_IRCCommand &);
        void        executeNick     (const t_IRCCommand &);
        void        executeUser     (const t_IRCCommand &);
        void        executeQuit     (const t_IRCCommand &);
        void        executeJoin     (const t_IRCCommand &);
        void        executePart     (const t_IRCCommand &);     
        void        executePrivMsg  (const t_IRCCommand &);
        void        executePing     (const t_IRCCommand &);

        void        setRole         (ARole* newRole);

    public:
        Client(int fd, char *host);
        Client (const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        int         get_fd() const;
        void        executeCommand(const std::string &message);
        bool        is_authenticated();

//----------Getters
        std::string getNickName() const;
        std::string getUserName() const;
        std::string getFullName() const;
        std::string getHostMask() const;
        std::string getPrefix() const;

//----------Setters
        void        setNickName(const std::string &nickName);
        void        setUserName(const std::string &username);
        void        setFullName(const std::string &fullname);
        
//----------Auth
        bool        _has_set_password;
};