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

/**
 * @brief Main class representing the server.
 * 
 * This class is responsible for managing client connections, communication, and server events.
 * It handles incoming connections, client disconnections, and message processing.
 */
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
        
//---------------------MAIN CORE---------------------------------------------------------
        void                serverLoop();
        void                handlePollEvent(size_t i);
        void                handleClientDisconnection(size_t i);
        void                handleNewConnection();
        void                handleClientMessage(size_t i);

//---------------------CLIENT HANDLING---------------------------------------------------
        void                addClient(Client *client);
        void                removeClient(Client* user, const std::string &reason);
        void                removeClientFromEveryChannels(Client* user, const std::string &reason);

//---------------------CHANNEL HANDLING--------------------------------------------------
        Channel*            createChannel(const std::string& channelName);
        void                removeChannel(Channel *channel);
        Channel*            findChannel(const std::string& channelName);
        Client*             findClientByNick(const std::string& target);
        void                broadcastMessage(const std::string& msg, Channel* channel);

//---------------------EXECUTION---------------------------------------------------------
        bool                executeCommand(Client* client, const std::string &message);
        std::string         getMessage(Client *client, Client *target, Channel *channel, const std::string &command, size_t code);
        void                privMsgBroadcast(const std::string& msg, Channel* channel, int sender_fd);

        typedef bool (Server::*CommandFunction)(Client*, const t_IRCCommand &);
        void                initializeCommandMap();
        std::map<std::string, CommandFunction>  _commandMap;

        bool                kick    (Client *, const t_IRCCommand &);
        bool                invite  (Client *, const t_IRCCommand &);
        bool                topic   (Client *, const t_IRCCommand &);
        bool                mode    (Client *, const t_IRCCommand &);
        bool                join    (Client *, const t_IRCCommand &);
        bool                part    (Client *, const t_IRCCommand &);
        bool                privMsg (Client *, const t_IRCCommand &);
        bool                ping    (Client *, const t_IRCCommand &);
        bool                cap     (Client *, const t_IRCCommand &);
        bool                pass    (Client *, const t_IRCCommand &);
        bool                user    (Client *, const t_IRCCommand &);
        bool                nick    (Client *, const t_IRCCommand &);
        bool                quit    (Client *, const t_IRCCommand &);
//---------------------------------------------------------------------------------------

        bool                isNickNameTaken(const std::string &nickName);

    public:
        Server(const std::string &password);
        ~Server();

        void                runServer(char **argv);
};