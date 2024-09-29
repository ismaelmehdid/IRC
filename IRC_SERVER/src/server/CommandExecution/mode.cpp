#include "../../../include/server/Server.hpp"

// + : turn on
// - : turn off

// i -> invite only
// t -> operator only can change the topic of the channel
// k -> channel protected by a password                     TAKE PARAMS IF +
// o -> set a user to be operator                           TAKE PARAMS ALWAYS
// l -> limit of users in channel                           TAKE PARAMS IF +

// MODE <channel> <modes> [<params>]

// Treated separatly

bool    Server::mode(Client *client, const t_IRCCommand &command)
{
    int         fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    std::string channelName = command.params[0];
    Channel     *channelToModify = this->findChannel(channelName);

    if (!channelToModify)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL)))
            return (false);
        return (true);
    }

    else if (command.params.size() < 1)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    if (command.params.size() == 1)
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", RPL_CHANNELMODEIS)))
            return (false);
        return (true);
    }

    std::string modesToChange = command.params[1];

    if (!channelToModify->isOperator(client))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_CHANOPRIVSNEEDED)))
            return (false);
        return (true);
    }

    if (!channelToModify->isMember(client))
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NOTONCHANNEL)))
            return (false);
        return (true);
    }

    size_t  parameter_index = 2;
    bool    addMode         = true; // toogle to check if we are adding or removing a mode
    Client  *targetToChange = NULL;

    //472 for unknown mode ERR_UNKNOWNMODE
    for (size_t i = 0; i != modesToChange.size(); i++)
    {
        switch (modesToChange[i])
        {
        case '+':
            addMode = true;
            break ;

        case '-':
            addMode = false;
            break ;

        case 'i':
            if ((addMode && channelToModify->isInviteOnly()) || (!addMode && !channelToModify->isInviteOnly()))
                break;
            channelToModify->setInviteOnly(addMode);
            if (addMode)
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +i\r\n"), channelToModify);
            else
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " -i\r\n"), channelToModify);
            break ;

        case 't':
            if ((addMode && channelToModify->isTopicLocked()) || (!addMode && !channelToModify->isTopicLocked()))
                break;
            channelToModify->setTopicLocked(addMode);
            if (addMode)
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +t\r\n"), channelToModify);
            else
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " -t\r\n"), channelToModify);
            break ;

        case 'k':

            if (addMode)
            {
                if (channelToModify->hasPassword())
                {
                    if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_KEYSET)))
                        return (false);
                }
                else if (parameter_index >= command.params.size())
                {
                    if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NEEDMOREPARAMS)))
                        return (false);
                }
                else
                {
                    channelToModify->setPassword(command.params[parameter_index]);
                    broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +k " + command.params[parameter_index] + "\r\n"), channelToModify);
                    parameter_index++;
                }
            }
            else
            {
                channelToModify->setPassword("");
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " -k *\r\n"), channelToModify);
            }
            break ;

        case 'o':
            if (parameter_index >= command.params.size())
            {
                if (!this->_socket.Send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NEEDMOREPARAMS)))
                    return (false);
                break ;
            }

            targetToChange = this->findClientByNick(command.params[parameter_index]);

            if (!targetToChange)
            {
                if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, command.params[parameter_index], ERR_NOSUCHNICK)))
                    return (false);
                break ;
            }

            if (!channelToModify->isMember(targetToChange))
            {
                if (!this->_socket.Send(fd, getMessage(client, targetToChange, channelToModify, "MODE", ERR_USERNOTINCHANNEL)))
                    return (false);
                break ;
            }

            if (addMode)
            {
                channelToModify->addOperator(targetToChange);
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +o " + targetToChange->getNickName() + "\r\n"), channelToModify);
            }
            else
            {
                channelToModify->removeOperator(targetToChange);
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " -o " + targetToChange->getNickName() + "\r\n"), channelToModify);
            }
            parameter_index++;
            break ;

        case 'l':
            if (addMode)
            {
                if (parameter_index >= command.params.size())
                {
                    if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS)))
                        return (false);
                }
                else
                {
                    int                 newLimit;
                    std::stringstream   ss(command.params[parameter_index]);
                    ss >> newLimit;

                    if (ss.fail() || !ss.eof()) {}
                    else
                    {
                        channelToModify->setUserLimit(newLimit);
                        broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +l " + command.params[parameter_index] + "\r\n"), channelToModify);
                        parameter_index++;
                    }
                }
            }
            else
            {
                channelToModify->setUserLimit(-1);
                broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " -l\r\n"), channelToModify);
            }
            break ;
        
        default:
            std::string unknownChar(1, modesToChange[i]);
            if (!this->_socket.Send(fd, getMessage(client, targetToChange, channelToModify, unknownChar, ERR_UNKNOWNMODE)))
                return (false);
            break ;
        }
    }
    return (true);
}