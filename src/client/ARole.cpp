#include "../../include/server/Server.hpp"

ARole::ARole() : _client(NULL) {}

ARole::ARole(Client *client) : _client(client) {}

ARole::~ARole() {}

void    ARole::quit(const t_IRCCommand &command)
{
	std::string reason = (command.params.empty()) ? "" : (":" + command.params[0]);

    std::cout << YELLOW << this->_client->getNickName() << " disconnected!" << RESET << std::endl;

	global_ircserv->socketSend(this->_client->get_fd(), SERVER_NAME " QUIT " + this->_client->getNickName() + " " + reason);
    global_ircserv->removeClient(this->_client, reason);
}

void    ARole::ping(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    global_ircserv->socketSend(this->_client->get_fd(), pong_message);
}

std::string ARole::getMessage(t_msgs message, Channel *channel)
{
    std::ostringstream  msg;
    const std::map<int, Client*> &clients = channel->getClients();

    switch (message)
    {
        case JOIN:
            msg << this->_client->getPrefix() << " JOIN " << channel->getName() << "\r\n";
            break;
        case NO_TOPIC:
            msg << ":" << SERVER_NAME << " 331 " << this->_client->getNickName() << " "
                << channel->getName() << " :No topic is set\r\n";
            break;
        case TOPIC:
            msg << ":" << SERVER_NAME << " 332 " << this->_client->getNickName() << " "
                << channel->getName() << " :" << channel->getTopic() << "\r\n";
            break;
        case NAMES_REPLY:
            msg << ":" << SERVER_NAME << " 353 " << this->_client->getNickName() << " = "
                << channel->getName() << " :";
            for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it != clients.begin())
                    msg << " ";
                msg << it->second->getNickName();
            }
            msg << "\r\n";
            break;
        case END_OF_NAMES:
            msg << ":" << SERVER_NAME << " 366 " << this->_client->getNickName() << " "
                << channel->getName() << " :End of NAMES list\r\n";
            break;
        case MODE:
            msg << "lol"; // Mode message
            break;
        case ALREADY_JOINED_ERROR:
            msg << ":" << SERVER_NAME << " " << ERR_ALREADY_JOINED << " "
                << this->_client->getNickName() << " " << channel->getName() << " :"
                << ERR_ALREADY_JOINED_MSG;
            break;
    }
    return (msg.str());
}

void    ARole::join(const t_IRCCommand &command)
{
    int     fd = this->_client->get_fd();

    if (command.params.empty())
    {
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    Channel *channel = global_ircserv->findChannel(command.params[0]);

    if (!channel)
    {
        channel = global_ircserv->createChannel(command.params[0]);
    }

    // checks for invite only password etc
    if (!channel->isMember(this->_client))
    {
        channel->addClient(this->_client);
        channel->broadcastMessage(getMessage(JOIN, channel));

        if (!channel->getTopic().empty())
        {
            global_ircserv->socketSend(fd, getMessage(TOPIC, channel));
        }
        else
        {
            global_ircserv->socketSend(fd, getMessage(NO_TOPIC, channel));
        }

        global_ircserv->socketSend(fd, getMessage(NAMES_REPLY, channel));
        global_ircserv->socketSend(fd, getMessage(END_OF_NAMES, channel));
        // mode msg
    }
    else
    {
        global_ircserv->socketSend(fd, getMessage(ALREADY_JOINED_ERROR, channel));
    }
}


void    ARole::part(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return;
    }

    const std::string   &channel_name = command.params[0];
    Channel             *channel = global_ircserv->findChannel(channel_name);
    
    if (!channel)
    {
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NO_SUCH_CHANNEL);
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
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &target = command.params[0];
    const std::string   &message = command.trailing;

    if (target[0] == '#')
    {
        Channel *channel = global_ircserv->findChannel(target);
        if (!channel)
        {
            global_ircserv->socketSend(this->_client->get_fd(), ERR_NO_SUCH_CHANNEL);
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
            global_ircserv->socketSend(this->_client->get_fd(), ERR_NO_SUCH_NICK);
            return;
        }
        std::string privmsg = ":" + this->_client->getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        global_ircserv->socketSend(target_client->get_fd(), privmsg);
    }
}
