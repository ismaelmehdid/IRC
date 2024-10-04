#pragma once

# include "Socket.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../irc.hpp"
# include "../exception/ServerBindException.hpp"
# include "../exception/ServerCreationException.hpp"
# include "../exception/ServerListenException.hpp"
# include "../exception/PollException.hpp"

# include <list>

//===----------------------------------------------------------------------===//

class Server
{
    private:
        Server(const Server &server);
        Server &operator=(const Server &server);

        unsigned int                    _nbr_clients;      ///< Number of connected clients
        std::string                     _password;         ///< Server password for client authentication
        std::map<int, Client*>          _clients;          ///< Map of connected clients, key is client file descriptor
        std::map<std::string, Channel*> _channels;         ///< Map of channels for managing group communications
        std::vector<pollfd>             _fds;              ///< Poll file descriptors for client sockets
        pollfd                          _server_pollfd;    ///< Pollfd structure for the server socket
        int                             _poll_count;       ///< Number of poll events
        Socket                          _socket;           ///< Socket instance for network operations
        std::vector<std::string>        _nicknames;
        
//=== Main core -----------------------------------------------------------===//
        void                serverLoop();
        void                handlePollEvent(size_t i);
        void                handleClientDisconnection(size_t i);
        void                handleNewConnection();
        void                handleClientMessage(size_t i);

//=== Client handling -----------------------------------------------------===//
        void                addClient(Client *client);
        void                removeClient(Client* user, const std::string &reason);
        void                removeClientFromEveryChannels(Client* user, const std::string &reason);

//=== Channel handling ----------------------------------------------------===//
        Channel*            createChannel(const std::string& channelName);
        void                removeChannel(Channel *channel);
        Channel*            findChannel(const std::string& channelName);
        Client*             findClientByNick(const std::string& target);
        void                broadcastMessage(const std::string& msg, Channel* channel);

//=== Execution -----------------------------------------------------------===//
        bool                executeCommand(Client* client, const std::string &message);
        void                privMsgBroadcast(const std::string& msg, Channel* channel, int sender_fd);

        typedef bool (Server::*CommandFunction)(Client*, const t_IRCCommand &);
        void                initializeCommandMap();
        std::map<std::string, CommandFunction>  _commandMap;

        bool                kick    (Client *, const t_IRCCommand &);
        bool                invite  (Client *, const t_IRCCommand &);
        bool                topic   (Client *, const t_IRCCommand &);

        bool                mode    (Client *, const t_IRCCommand &);
        bool                handleModesToChange(t_ModeCommandData &);
        void                handle_i(t_ModeCommandData &);
        void                handle_t(t_ModeCommandData &);
        bool                handle_k(t_ModeCommandData &);
        bool                handle_o(t_ModeCommandData &);
        bool                handle_l(t_ModeCommandData &);

        bool                join    (Client *, const t_IRCCommand &);
        bool                part    (Client *, const t_IRCCommand &);
        bool                privMsg (Client *, const t_IRCCommand &);
        bool                ping    (Client *, const t_IRCCommand &);
        bool                cap     (Client *, const t_IRCCommand &);
        bool                pass    (Client *, const t_IRCCommand &);
        bool                user    (Client *, const t_IRCCommand &);
        bool                nick    (Client *, const t_IRCCommand &);
        bool                quit    (Client *, const t_IRCCommand &);

        bool                isNickNameTaken(const std::string &nickName);

    public:
        Server(const std::string &password);
        ~Server();

        void                runServer(char **argv);
        std::string         getMessage(Client *client, Client *target, Channel *channel, const std::string &command, size_t code);
};

//===----------------------------------------------------------------------===//