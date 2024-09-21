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
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, ERR_NOTREGISTERED);
        return;
    }

    if (command.params.size() < 2)
    {
        this->_socket.send(fd, ERR_NEED_MORE_PARAMS);
        return;
    }

    std::string channelName = command.params[0];
    std::string modesToChange = command.params[1];

    Channel *channelToModify = this->findChannel(channelName);
    if (!channelToModify)
    {
        this->_socket.send(fd, ERR_NO_SUCH_CHANNEL);
        return;
    }

    if (!channelToModify->isOperator(client))
    {
        this->_socket.send(fd, getMessage(client, ERR_CHANOPRIVSNEEDED, channelToModify));
        return;
    }

    if (!channelToModify->isMember(client))
    {
        // send ERR_NOTONCHANNEL 442
        return;
    }

    size_t  parameter_index = 2;
    bool    addMode = true; // toogle to check if we are adding or removing a mode

    //472 for unknown mode ERR_UNKNOWNMODE
    for (int i = 0; i != modesToChange.size(); i++) {
        switch (modesToChange[i])
        {
        case '+':
            addMode = true;
            break;
        case '-':
            addMode = false;
            break;
        case 'i':
            channelToModify->setInviteOnly(addMode);
            break;
        case 't':
            channelToModify->setTopicLocked(addMode);
            break;
        case 'k':
            if (addMode) {
                if (channelToModify->hasPassword()) {
                    // send 467
                } else if (parameter_index >= command.params.size()) {
                    // need more params
                } else {
                    channelToModify->setPassword(command.params[parameter_index]);
                    // send password set
                    parameter_index++;
                }
            } else {
                channelToModify->setPassword("");
                // send password removed
            }
            break;
        case 'o':
            if (parameter_index >= command.params.size()) {
                // need more params
                break;
            }
            Client *targetToChange = this->findClientByNick(command.params[parameter_index]);
            if (!targetToChange) {
                this->_socket.send(fd, getMessage(client, ERR_NOSUCHNICK, channelToModify)); // TO CLEAN
                break;
            }

            if (!channelToModify->isMember(targetToChange)) {
                this->_socket.send(fd, getMessage(client, ERR_USERNOTINCHANNEL, channelToModify));
                break;
            }

            if (addMode) {
                channelToModify->addOperator(targetToChange);
                // send gives channel operator status
            } else {
                channelToModify->removeOperator(targetToChange);
                // send removes channel operator status
            }
            parameter_index++;
            break;
        case 'l':
            if (addMode) {
                if (parameter_index >= command.params.size()) {
                    // need more params
                } else {
                    int newLimit;
                    std::stringstream ss(command.params[parameter_index]);
                    ss >> newLimit;

                    if (ss.fail() || !ss.eof()) {
                        //nothing to do apparently 
                    } else {
                        channelToModify->setUserLimit(newLimit);
                        parameter_index++;
                        // send limit added
                    }
                }
            } else {
                channelToModify->setUserLimit(-1);
                // send removes user limit
            }
            break;
        default:
            // send 472
            break;
        }
    }
}