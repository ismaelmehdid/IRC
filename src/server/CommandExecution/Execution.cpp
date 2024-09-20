#include "../../../include/server/Server.hpp"

// void Server::PermissionDenied(Client* client, const std::string &nickName)
// {
//     this->_socket.send(client->get_fd(), ":server 481 " + nickName + ": Permission Denied - You're not an IRC operator.\r\n");
// }

void    Server::executeCommand(Client* client, const std::string &message)
{
    std::vector<t_IRCCommand> parsed_commands = parseRequests(message);

    for (size_t i = 0; i < parsed_commands.size(); i++)
    {
        std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
        if (it != _commandMap.end())
        {
            // Call the command function, passing the client and the parsed command
            (this->*(it->second))(client, parsed_commands[i]);
        }
        else
        {
            this->_socket.send(client->get_fd(), ERR_UNKNOWNCOMMAND);
        }
    }
}

std::string Server::getMessage(Client *client, t_msgs message, Channel *channel)
{
    std::ostringstream      msg;
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
                if (channel->isOperator(*it))
                    msg << "@" << (*it)->getNickName(); 
                else
                    msg << (*it)->getNickName();
            }
            msg << "\r\n";
            break;
        case END_OF_NAMES:
            msg << ":" << SERVER_NAME << " 366 " << client->getNickName() << " " << channel->getName()
                << " :End of /NAMES list. Operators: " << channel->getOperators().size()
                << ", Users: " << channel->getNbrUsers() << "\r\n";
            break;
        case MODE:
            msg << ":" << SERVER_NAME << " MODE " << channel->getName() << " +";
            if (channel->isInviteOnly()) msg << "i";
            if (channel->hasPassword()) msg << "k";
            if (channel->getUserLimit() != -1) msg << "l";
            msg << "\r\n";
            break;
        case ALREADY_JOINED_ERROR:
            msg << ":" << SERVER_NAME << " " << ERR_ALREADY_JOINED << " "
                << client->getNickName() << " " << channel->getName() << " :"
                << ERR_ALREADY_JOINED_MSG;
            break;
        case ERR_MODE_I:
            msg << ":" << SERVER_NAME << " 473 "
                << client->getNickName() << " " << channel->getName() << " :Cannot join channel (+i) - Invite-only\r\n";
            break ;
        case ERR_MODE_L:
            msg << ":" << SERVER_NAME << " 471 "
                << client->getNickName() << " " << channel->getName() << " :Cannot join channel (+l) - Channel is full\r\n";
            break ;
        case ERR_MODE_K:
            msg << ":" << SERVER_NAME << " 475 "
                << client->getNickName() << " " << channel->getName() << " :Cannot join channel (+k) - Password required\r\n";
            break ;
    }
    return (msg.str());
}