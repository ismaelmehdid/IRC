#include "../../include/server/Server.hpp"

Server::Server(const std::string &password)
    :   _nbr_clients(0),
        _password(password),
        _clients(),
        _channels(),
        _fds(),
        _server_pollfd(),
        _poll_count(0),
        _socket() {}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        delete (it->second);
    }

    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        delete (it->second);
    }

    for (int i = _poll_count - 1; i >= 0; --i)
    {
        pollRemove(i);
    }
}

const std::string   Server::get_password() const
{
    return (this->_password);
}

bool    Server::isNickNameTaken(const std::string &nickName)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNickName() == nickName)
        {
            return (true);
        }
    }
    return (false);
}

bool    Server::socketSend(int fd, const std::string& message)
{
    bool    res = this->_socket.send(fd, message);
    
    return (res);
}

/**
 * @brief The main server loop for handling connections and events.
 * 
 * This function continuously polls the file descriptors in the _fds array for events.
 * If poll fails, it throws a PollException. For each event, it delegates to handlePollEvent().
 */
void    Server::serverLoop()
{
    while (true)
    {
        this->_poll_count = poll(_fds.data(), _fds.size(), -1);

        if (this->_poll_count == -1)
            throw PollException();

        for (size_t i = 0; i < _fds.size(); i++)
        {
            handlePollEvent(i);
        }
    }
}

/**
 * @brief Starts the server and sets it up for accepting connections.
 * 
 * This function creates the server socket, binds it to a port, and starts listening for connections.
 * It sets up the server file descriptor in the pollfd array and enters the ServerLoop() to begin processing connections.
 * 
 * @param argv Command-line arguments. argv[1] should contain the port number to bind to.
 * @throws ServerCreationException if socket creation fails.
 * @throws ServerBindException if binding the socket fails.
 * @throws ServerListenException if the server fails to start listening for connections.
 */
void    Server::runServer(char **argv)
{
    if (!this->_socket.create())
        throw ServerCreationException();
    if (!this->_socket.bind(std::strtol(argv[1], NULL, 10)))
        throw ServerBindException();
    if (!this->_socket.listen())
        throw ServerListenException();

    this->_server_pollfd.fd = this->_socket.get_fd();
    this->_server_pollfd.events = POLLIN;
    this->_fds.push_back(this->_server_pollfd);

    std::cout << "Waiting for connections on port "
              << argv[1] << "..." << std::endl;

    serverLoop();
}

// --------------------- COMMAND EXECUTION --------------------- 

// static void send_permission_denied(int fd, const std::string &nickName)
// {
//     global_ircserv->socketSend(fd, ":server 481 " + nickName + ": Permission Denied - You're not an IRC operator.\r\n");
// }

void Server::kick(Client *client, const t_IRCCommand &command)
{
    (void)client;
    (void)command;
}

void Server::invite(Client *client, const t_IRCCommand &command)
{
    (void)client;
    (void)command;
}

void Server::topic(Client *client, const t_IRCCommand &command)
{
    (void)client;
    (void)command;
}

void Server::mode(Client *client, const t_IRCCommand &command)
{
    (void)client;
    (void)command;
}

void Server::join(Client *client, const t_IRCCommand &command)
{
    int     fd = client->get_fd();

    if (command.params.empty())
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    Channel *channel = global_ircserv->findChannel(command.params[0]);

    if (!channel)
    {
        channel = global_ircserv->createChannel(command.params[0]);
    }

    // checks for invite only password etc
    if (!channel->isMember(client))
    {
        channel->addClient(client);
        channel->broadcastMessage(getMessage(client, JOIN, channel));

        if (!channel->getTopic().empty())
        {
            global_ircserv->socketSend(fd, getMessage(client, TOPIC, channel));
        }
        else
        {
            global_ircserv->socketSend(fd, getMessage(client, NO_TOPIC, channel));
        }

        global_ircserv->socketSend(fd, getMessage(client, NAMES_REPLY, channel));
        global_ircserv->socketSend(fd, getMessage(client, END_OF_NAMES, channel));
        // mode msg
    }
    else
    {
        global_ircserv->socketSend(fd, getMessage(client, ALREADY_JOINED_ERROR, channel));
    }
}

void Server::part(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return;
    }

    const std::string   &channel_name = command.params[0];
    Channel             *channel = global_ircserv->findChannel(channel_name);
    
    if (!channel)
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NO_SUCH_CHANNEL);
        return;
    }

    channel->removeClient(client);
    std::string         part_message = ":" + client->getNickName() + " PART " + channel_name + "\r\n";
    channel->broadcastMessage(part_message);
}

void Server::privMsg(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty() || command.trailing.empty())
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &target = command.params[0];
    const std::string   &message = command.trailing;

    if (target[0] == '#')
    {
        Channel *channel = global_ircserv->findChannel(target);
        if (!channel)
        {
            global_ircserv->socketSend(client->get_fd(), ERR_NO_SUCH_CHANNEL);
            return ;
        }

        std::string     privMsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        channel->broadcastMessage(privMsg);
    }
    else
    {
        Client *target_client = global_ircserv->findClientByNick(target);
        if (!target_client)
        {
            global_ircserv->socketSend(client->get_fd(), ERR_NO_SUCH_NICK);
            return;
        }
        std::string privmsg = ":" + client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        global_ircserv->socketSend(target_client->get_fd(), privmsg);
    }
}

void Server::ping(Client *client, const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    global_ircserv->socketSend(client->get_fd(), pong_message);
}

void Server::cap(Client *client, const t_IRCCommand &command)
{
	if (command.params[0] == "LS")
	{
         // server doesn't have any special features
        std::string empty_list_response = ":server CAP " + client->getNickName() + "LS :\r\n";
        global_ircserv->socketSend(client->get_fd(), empty_list_response);
	}
}

void Server::pass(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated())
    {
        if (command.params.empty())
        {
            global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else if (command.params[0] != global_ircserv->get_password())
        {
            global_ircserv->socketSend(client->get_fd(), ERR_PASSWORD_INCORRECT);
        }
        else
        {
            client->_has_set_password = true;
            //TODO: Notify that the client have put the right password
        }
    }
    else
    {
        global_ircserv->socketSend(client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

void Server::user(Client *client, const t_IRCCommand &command)
{
    if (!client->is_authenticated() && client->getUserName().empty() && client->getFullName().empty())
    {
        if (!client->_has_set_password)
        {
            global_ircserv->socketSend(client->get_fd(), ERR_PASSWORD_REQUIRED);
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else
        {
            client->setUserName(command.params[0]);
            client->setFullName(command.trailing);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;

            if (client->is_authenticated()) {
                global_ircserv->socketSend(client->get_fd(),
                    client->getPrefix() + WELCOME_CODE + client->getNickName() + MSG_WELCOME);
            }
        }
    }
    else
    {
        global_ircserv->socketSend(client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

static bool is_nickname_valid(const std::string &nick)
{
    if (nick.size() > MAX_NICKNAME_LENGTH)
        return (false);
    if (std::isdigit(nick[0]) || nick[0] == '#' || nick[0] == '&')
        return (false);
    for (size_t i = 1; i < nick.size(); i++)
    {
        if (!std::isalnum(nick[0]) &&
            nick[0] != '-' &&
            nick[0] != '[' &&
            nick[0] != ']' &&
            nick[0] != '\\' &&
            nick[0] != '^' &&
            nick[0] != '_' &&
            nick[0] != '{' &&
            nick[0] != '}'
            )
            return (false);
    }
    return (true);
}


void Server::nick(Client *client, const t_IRCCommand &command)
{
    if (!client->_has_set_password)
    {
        global_ircserv->socketSend(client->get_fd(), ERR_PASSWORD_REQUIRED);
    } 
    else if (command.params.empty())
    {
        global_ircserv->socketSend(client->get_fd(), ERR_NEED_MORE_PARAMS);
    }
    else
    {
        std::string newNick = command.params[0];
        if (client->getNickName() == newNick)
            return ;
        if (!is_nickname_valid(newNick))
        {
            global_ircserv->socketSend(client->get_fd(), ":server 432 * " + newNick + " :Erroneous nickname\r\n");
        }
        else if (global_ircserv->isNickNameTaken(newNick))
        {
            global_ircserv->socketSend(client->get_fd(), ERR_NICKNAME_IN_USE);
        }
        else
        {
            // TODO: broadcast new nickname to all users in channel
            bool wasEmpty = client->getNickName().empty();
            client->setNickName(newNick);
            if (wasEmpty && client->is_authenticated()) {
                global_ircserv->socketSend(client->get_fd(),
                    client->getPrefix() + WELCOME_CODE + client->getNickName() + MSG_WELCOME);
            }
        }
    }
}

void Server::quit(Client *client, const t_IRCCommand &command)
{
	std::string reason = (command.params.empty()) ? "" : (":" + command.params[0]);

    std::cout << YELLOW << client->getNickName() << " disconnected!" << RESET << std::endl;

	global_ircserv->socketSend(client->get_fd(), SERVER_NAME " QUIT " + client->getNickName() + " " + reason);
    global_ircserv->removeClient(client, reason);
}

std::string Server::getMessage(Client *client, t_msgs message, Channel *channel)
{
    std::ostringstream  msg;
    const std::set<Client*> &clients = channel->getClients();

    switch (message)
    {
        case JOIN:
            msg << client->getPrefix() << " JOIN " << channel->getName() << "\r\n";
            break;
        case NO_TOPIC:
            msg << ":" << SERVER_NAME << " 331 " << client->getNickName() << " "
                << channel->getName() << " :No topic is set\r\n";
            break;
        case TOPIC:
            msg << ":" << SERVER_NAME << " 332 " << client->getNickName() << " "
                << channel->getName() << " :" << channel->getTopic() << "\r\n";
            break;
        case NAMES_REPLY:
            msg << ":" << SERVER_NAME << " 353 " << client->getNickName() << " = "
                << channel->getName() << " :";
            for (std::set<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it != clients.begin())
                    msg << " ";
                msg << (*it)->getNickName();
            }
            msg << "\r\n";
            break;
        case END_OF_NAMES:
            msg << ":" << SERVER_NAME << " 366 " << client->getNickName() << " "
                << channel->getName() << " :End of NAMES list\r\n";
            break;
        case MODE:
            msg << "lol"; // Mode message
            break;
        case ALREADY_JOINED_ERROR:
            msg << ":" << SERVER_NAME << " " << ERR_ALREADY_JOINED << " "
                << client->getNickName() << " " << channel->getName() << " :"
                << ERR_ALREADY_JOINED_MSG;
            break;
    }
    return (msg.str());
}