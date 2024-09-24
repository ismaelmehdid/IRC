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

void    Server::mode(Client *client, const t_IRCCommand &command)
{
    int         fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NOTREGISTERED));
        return ;
    }

    if (command.params.size() < 1)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS));
        return ;
    }

    std::string channelName = command.params[0];
    Channel     *channelToModify = this->findChannel(channelName);

    if (!channelToModify)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL));
        return ;
    }

    if (command.params.size() == 1)
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToModify, "JOIN", RAW_MODE));
        return ;
    }
    else if (command.params.size() < 2)
    {
        this->_socket.send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS));
        return ;
    }

    std::string modesToChange = command.params[1];

    if (!channelToModify->isOperator(client))
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_CHANOPRIVSNEEDED));
        return ;
    }

    if (!channelToModify->isMember(client))
    {
        this->_socket.send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NOTONCHANNEL));
        return ;
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
            channelToModify->setInviteOnly(addMode);
            break ;

        case 't':
            channelToModify->setTopicLocked(addMode);
            break ;

        case 'k':

            if (addMode)
            {
                if (channelToModify->hasPassword())
                {
                    this->_socket.send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_KEYSET));
                }
                else if (parameter_index >= command.params.size())
                {
                    this->_socket.send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NEEDMOREPARAMS));
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
                this->_socket.send(fd, getMessage(client, NULL, channelToModify, "MODE", ERR_NEEDMOREPARAMS));
                break ;
            }

            targetToChange = this->findClientByNick(command.params[parameter_index]);

            if (!targetToChange)
            {
                this->_socket.send(fd, getMessage(client, NULL, NULL, command.params[parameter_index], ERR_NOSUCHNICK)); // TO CLEAN
                break ;
            }

            if (!channelToModify->isMember(targetToChange))
            {
                this->_socket.send(fd, getMessage(client, targetToChange, channelToModify, "MODE", ERR_USERNOTINCHANNEL));
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
                    this->_socket.send(fd, getMessage(client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS));
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
                        parameter_index++;
                        broadcastMessage((client->getPrefix() + " MODE " + channelToModify->getName() + " +l " + command.params[parameter_index] + "\r\n"), channelToModify);
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
            this->_socket.send(fd, getMessage(client, targetToChange, channelToModify, unknownChar, ERR_UNKNOWNMODE));
            break ;
        }
    }
}