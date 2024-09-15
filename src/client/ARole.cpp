#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"
#include "../../include/client/ARole.hpp"
#include "../../include/server/Server.hpp"

ARole::ARole() : _client(NULL) { }

ARole::ARole(Client *client) : _client(client) { }

ARole::~ARole() { }

/**
 * @brief Passes a command to the ARole object.
 *
 * This function is responsible for handling the pass command received by the ARole object.
 * It checks if the client is authenticated and performs the necessary actions based on the command parameters.
 * If the client is not authenticated, it checks if the command parameters are empty or if the password is incorrect.
 * If the password is correct, it sets the _has_set_password flag to true.
 * If the client is already registered, it sends an error message to the client.
 *
 * @param command The IRC command to be passed.
 */
void ARole::pass(const t_IRCCommand &command)
{
    if (!_client->is_authenticated())
    {
        if (command.params.empty())
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else if (command.params[0] != global_ircserv->get_password())
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        }
        else
        {
            _client->_has_set_password = true;
        }
    }
    else
    {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

/**
 * @brief Changes the nickname of the client.
 * 
 * This function is used to change the nickname of the client. If the client is not authenticated,
 * it checks if a password has been set. If a password has not been set, it sends an error message
 * indicating that the password is incorrect. If the command parameters are empty, it sends an error
 * message indicating that more parameters are needed. If the nickname is already taken by another
 * client, it sends an error message indicating that the nickname is in use. Otherwise, it sets the
 * new nickname for the client.
 * 
 * If the client is already authenticated, it sends an error message indicating that the client is
 * already registered.
 * 
 * @param command The IRC command containing the new nickname.
 */
void ARole::nick(const t_IRCCommand &command)
{
    if (!_client->is_authenticated())
    {
        if (!_client->_has_set_password)
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        } 
        else if (command.params.empty())
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else
        {
            std::string newNick = command.params[0];
            if (global_ircserv->isNickNameTaken(newNick))
            {
                global_ircserv->_socket.send(_client->get_fd(), ERR_NICKNAME_IN_USE);
            }
            else
            {
                _client->setNickName(newNick);
            }
        }
    }
    else
    {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

/**
 * @brief Handles the user command received from the client.
 * 
 * This function is responsible for processing the user command received from the client.
 * It checks if the client is authenticated and if the necessary parameters are provided.
 * If the client is not authenticated and the required parameters are provided, it sets the client's username and full name,
 * sends a welcome message to the client, and marks the client as registered.
 * If the client is already registered, it sends an error message indicating that the client is already registered.
 * 
 * @param command The IRC command received from the client.
 */
void ARole::user(const t_IRCCommand &command)
{
    if (!_client->is_authenticated() && _client->getUserName().empty() && _client->getFullName().empty())
    {
        if (!_client->_has_set_password)
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        }
        else if (_client->getNickName().empty())
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else
        {
            _client->setUserName(command.params[0]);
            _client->setFullName(command.trailing);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;
            
            global_ircserv->_socket.send(_client->get_fd(), ":server 001 " + this->_client->getNickName() + " :Welcome to the Internet Relay Network " + this->_client->getNickName() + "!" + this->_client->getUserName() + "@" + this->_client->getHostMask() + "\r\n");
        }
    }
    else
    {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

void ARole::join(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->_socket.send(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return;
    }

    const std::string &channelName = command.params[0];
    Channel *channel = global_ircserv->findChannel(channelName);

    if (!channel)
    {
        channel = global_ircserv->createChannel(channelName);
    }

    if (!channel->isMember(this->_client))
    {
        channel->addClient(this->_client);

        std::string joinMessage;
        joinMessage.append(":").append(this->_client->getNickName()).append("!")
                   .append(this->_client->getUserName()).append("@:")
                   .append(SERVER_NAME).append(" JOIN ")
                   .append(channelName).append("\r\n");
        channel->broadcastMessage(joinMessage);

        std::string topicMessage;
        if (!channel->getTopic().empty())
        {
            topicMessage.append(":").append(SERVER_NAME).append(" 332 ")
                        .append(this->_client->getNickName()).append(" ")
                        .append(channelName).append(" :")
                        .append(channel->getTopic()).append("\r\n");
        }
        else
        {
            topicMessage.append(":").append(SERVER_NAME).append(" 331 ")
                        .append(this->_client->getNickName()).append(" ")
                        .append(channelName).append(" :No topic is set\r\n");
        }
        global_ircserv->_socket.send(this->_client->get_fd(), topicMessage);

        std::string namesReply;
        namesReply.append(":").append(SERVER_NAME).append(" 353 ")
                  .append(this->_client->getNickName()).append(" = ")
                  .append(channelName).append(" :");
        const std::map<int, Client*> &clients = channel->getClients();
        for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it != clients.begin())
                namesReply.append(" ");
            namesReply.append(it->second->getNickName());
        }
        namesReply.append("\r\n");
        global_ircserv->_socket.send(this->_client->get_fd(), namesReply);

        std::string endOfNames;
        endOfNames.append(":").append(SERVER_NAME).append(" 366 ")
                  .append(this->_client->getNickName()).append(" ")
                  .append(channelName).append(" :End of NAMES list\r\n");
        global_ircserv->_socket.send(this->_client->get_fd(), endOfNames);
    }
    else
    {
        std::string errorMsg;
        errorMsg.append(":").append(SERVER_NAME).append(" ")
                .append(ERR_ALREADY_JOINED).append(" ")
                .append(this->_client->getNickName()).append(" ")
                .append(channelName).append(" :")
                .append(ERR_ALREADY_JOINED_MSG);
        global_ircserv->_socket.send(this->_client->get_fd(), errorMsg);
    }
}

void    ARole::part(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->_socket.send(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return;
    }

    const std::string   &channel_name = command.params[0];
    Channel             *channel = global_ircserv->findChannel(channel_name);
    
    if (!channel)
    {
        global_ircserv->_socket.send(this->_client->get_fd(), ERR_NO_SUCH_CHANNEL);
        return;
    }

    channel->removeClient(this->_client);
    std::string         part_message = ":" + this->_client->getNickName() + " PART " + channel_name + "\r\n";
    channel->broadcastMessage(part_message);
}

void    ARole::privMsg(const t_IRCCommand &command)
{
    if (command.params.empty() || command.trailing.empty())
    {
        global_ircserv->_socket.send(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &target = command.params[0];
    const std::string   &message = command.trailing;

    if (target[0] == '#')
    {
        Channel         *channel = global_ircserv->findChannel(target);
        if (!channel)
        {
            global_ircserv->_socket.send(this->_client->get_fd(), ERR_NO_SUCH_CHANNEL);
            return ;
        }

        std::string     privMsg = ":" + this->_client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        channel->broadcastMessage(privMsg);
    }
    else
    {
        Client *target_client = global_ircserv->findClientByNick(target);
        if (!target_client)
        {
            global_ircserv->_socket.send(this->_client->get_fd(), ERR_NO_SUCH_NICK);
            return;
        }

        std::string privmsg = ":" + this->_client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        global_ircserv->_socket.send(target_client->get_fd(), privmsg);
    }
}

void    ARole::ping(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->_socket.send(this->_client->get_fd(), ":server 461 * PING :Not enough parameters\r\n");
        return ;
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    global_ircserv->_socket.send(this->_client->get_fd(), pong_message);
}





