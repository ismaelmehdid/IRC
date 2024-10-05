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

void    Server::handle_i(t_ModeCommandData &data)
{
    if ((data.addMode && data.channelToModify->isInviteOnly()) || (!data.addMode && !data.channelToModify->isInviteOnly()))
        return;
    data.channelToModify->setInviteOnly(data.addMode);
    if (data.addMode)
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " +i\r\n"), data.channelToModify);
    else
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " -i\r\n"), data.channelToModify);
}

void    Server::handle_t(t_ModeCommandData &data)
{
    if ((data.addMode && data.channelToModify->isTopicLocked()) || (!data.addMode && !data.channelToModify->isTopicLocked()))
        return;
    data.channelToModify->setTopicLocked(data.addMode);
    if (data.addMode)
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " +t\r\n"), data.channelToModify);
    else
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " -t\r\n"), data.channelToModify);
}

bool    Server::handle_k(t_ModeCommandData &data)
{
    if (data.addMode)
    {
        if (data.channelToModify->hasPassword())
        {
            if (!this->_socket.Send(data.client_fd, getMessage(data.client, NULL, data.channelToModify, "MODE", ERR_KEYSET)))
                return (false);
        }
        else if (data.parameter_index >= data.command.params.size())
        {
            if (!this->_socket.Send(data.client_fd, getMessage(data.client, NULL, data.channelToModify, "MODE", ERR_NEEDMOREPARAMS)))
                return (false);
        }
        else
        {
            data.channelToModify->setPassword(data.command.params[data.parameter_index]);
            broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " +k " + data.command.params[data.parameter_index] + "\r\n"), data.channelToModify);
            data.parameter_index++;
        }
    }
    else
    {
        data.channelToModify->setPassword("");
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " -k *\r\n"), data.channelToModify);
    }
    return (true);
}

bool    Server::handle_o(t_ModeCommandData &data)
{
    if (data.parameter_index >= data.command.params.size())
    {
        if (!this->_socket.Send(data.client_fd, getMessage(data.client, NULL, data.channelToModify, "MODE", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    data.target_to_change = this->findClientByNick(data.command.params[data.parameter_index]);

    if (!data.target_to_change)
    {
        if (!this->_socket.Send(data.client_fd, getMessage(data.client, NULL, NULL, data.command.params[data.parameter_index], ERR_NOSUCHNICK)))
            return (false);
        return (true);
    }

    if (!data.channelToModify->isMember(data.target_to_change))
    {
        if (!this->_socket.Send(data.client_fd, getMessage(data.client, data.target_to_change, data.channelToModify, "MODE", ERR_USERNOTINCHANNEL)))
            return (false);
        return (true);
    }

    if (data.addMode)
    {
        data.channelToModify->addOperator(data.target_to_change);
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " +o " + data.target_to_change->getNickName() + "\r\n"), data.channelToModify);
    }
    else
    {
        data.channelToModify->removeOperator(data.target_to_change);
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " -o " + data.target_to_change->getNickName() + "\r\n"), data.channelToModify);
    }
    data.parameter_index++;
    return (true);
}

bool    Server::handle_l(t_ModeCommandData &data)
{
    if (data.addMode)
    {
        if (data.parameter_index >= data.command.params.size())
        {
            if (!this->_socket.Send(data.client_fd, getMessage(data.client, NULL, NULL, "MODE", ERR_NEEDMOREPARAMS)))
                return (false);
        }
        else
        {
            int                 newLimit;
            std::stringstream   ss(data.command.params[data.parameter_index]);
            ss >> newLimit;

            if (ss.fail() || !ss.eof()) {}
            else
            {
                if (newLimit < 1) {
                    data.parameter_index++;
                    return (true);
                }
                data.channelToModify->setUserLimit(newLimit);
                broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " +l " + data.command.params[data.parameter_index] + "\r\n"), data.channelToModify);
                data.parameter_index++;
            }
        }
    }
    else
    {
        data.channelToModify->setUserLimit(-1);
        broadcastMessage((data.client->getPrefix() + " MODE " + data.channelToModify->getName() + " -l\r\n"), data.channelToModify);
    }
    return (true);
}

bool     Server::handleModesToChange(t_ModeCommandData &data)
{
    for (size_t i = 0; i != data.modesToChange.size(); i++)
    {
        switch (data.modesToChange[i])
        {
        case '+':
            data.addMode = true;
            break ;

        case '-':
            data.addMode = false;
            break ;

        case 'i':
            handle_i(data);
            break ;

        case 't':
            handle_t(data);
            break ;

        case 'k':
            if (!handle_k(data))
                return (false);
            break ;

        case 'o':
            if (!handle_o(data))
                return (false);
            break ;

        case 'l':
            if (!handle_l(data))
                return (false);
            break ;
        
        default:
            std::string unknownChar(1, data.modesToChange[i]);
            if (!this->_socket.Send(data.client_fd, global_ircserv->getMessage(data.client, data.target_to_change, data.channelToModify, unknownChar, ERR_UNKNOWNMODE)))
                return (false);
            break ;
        }
    }
    return (true);
}

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

    t_ModeCommandData data;
    data.client = client;
    data.client_fd = fd;
    data.channelToModify = channelToModify;
    data.modesToChange = modesToChange;
    data.parameter_index = 2;
    data.target_to_change = NULL;
    data.addMode = true;
    data.command = command;

    return handleModesToChange(data);
}