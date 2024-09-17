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

void    ARole::join(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(this->_client->get_fd(), ERR_NEED_MORE_PARAMS);
        return ;
    }

    const std::string   &channelName = command.params[0];
    Channel             *channel = global_ircserv->findChannel(channelName);

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
        global_ircserv->socketSend(this->_client->get_fd(), topicMessage);

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
        global_ircserv->socketSend(this->_client->get_fd(), namesReply);

        std::string endOfNames;
        endOfNames.append(":").append(SERVER_NAME).append(" 366 ")
                  .append(this->_client->getNickName()).append(" ")
                  .append(channelName).append(" :End of NAMES list\r\n");
        global_ircserv->socketSend(this->_client->get_fd(), endOfNames);
    }
    else
    {
        std::string errorMsg;
        errorMsg.append(":").append(SERVER_NAME).append(" ")
                .append(ERR_ALREADY_JOINED).append(" ")
                .append(this->_client->getNickName()).append(" ")
                .append(channelName).append(" :")
                .append(ERR_ALREADY_JOINED_MSG);
        global_ircserv->socketSend(this->_client->get_fd(), errorMsg);
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

void    ARole::ping(const t_IRCCommand &command)
{
    if (command.params.empty())
    {
        global_ircserv->socketSend(this->_client->get_fd(), ":server 461 * PING :Not enough parameters\r\n");
        return ;
    }

    const std::string   &server_name = command.params[0];
    std::string         pong_message = ":" + server_name + " PONG " + server_name + "\r\n";

    global_ircserv->socketSend(this->_client->get_fd(), pong_message);
}





