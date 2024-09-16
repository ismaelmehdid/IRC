#pragma once

# include "OperatorRole.hpp"
# include "RegularRole.hpp"

# include <string>
# include <map>

class ARole;

class Client
{
    public:
        Client(int fd);
        Client (const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        int         get_fd() const;
        void        execute_command(const std::string &message);
        bool        is_authenticated();

        // Getters
        std::string getNickName();
        std::string getUserName();
        std::string getFullName();

        //Setters
        void        setNickName(const std::string &nickName);
        void        setUserName(const std::string &username);
        void        setFullName(const std::string &fullname);
        
        //Auth
        bool                                    _has_set_password;

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
        void        executeQuit     (const t_IRCCommand &);

        void        setRole         (ARole* newRole);

        std::string                             _nickName; // the server must check uniqueness
        std::string                             _userName;
        std::string                             _fullName;
        int                                     _fd;
        ARole*                                  _role;
        std::map<std::string, CommandFunction>  _commandMap; // map associating every command string to the proper function ptr
};