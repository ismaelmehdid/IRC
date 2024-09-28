#include "../../../include/server/Server.hpp"

static bool IsChannelNameValid(const std::string &name)
{
    if (name.empty() || name[0] != '#' || name.size() < 2 || name.size() > MAX_CHANNEL_NAME_LENGTH)
        return (false);

    std::string channelName = name.substr(1);
    
    for (size_t i = 0; i < channelName.size(); ++i)
    {
        char c = channelName[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.'))
        {
            return (false);
        }
    }
    return (true);
}

static void splitByComma(const std::string &str, std::vector<std::string> &result)
{
    std::stringstream   ss(str);
    std::string         item;

    while (std::getline(ss, item, ','))
    {
        result.push_back(item);
    }
}


bool    Server::join(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "JOIN", ERR_NOTREGISTERED)))
            return (false);
        return (true);
    }

    if (command.params.empty())
    {
        if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, "JOIN", ERR_NEEDMOREPARAMS)))
            return (false);
        return (true);
    }

    std::string channelNames = command.params[0];
    std::transform(channelNames.begin(), channelNames.end(), channelNames.begin(), ::tolower);
    
    std::string passwords = (command.params.size() > 1) ? command.params[1] : "";

    std::vector<std::string>    channels;
    std::vector<std::string>    passList;
    splitByComma(channelNames,  channels);
    splitByComma(passwords,     passList);

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
        std::string password = (i < passList.size()) ? passList[i] : "";

        if (!IsChannelNameValid(channelName))
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, NULL, channelName, ERR_NOSUCHCHANNEL)))
                return (false);
            continue ;
        }

        Channel *channel = this->findChannel(channelName);

        if (channel && channel->isMember(client))
            continue ;

        if (!channel)
        {
            channel = this->createChannel(channelName);
            channel->addClient(client);
            channel->addOperator(client);
            if (!password.empty())
                channel->setPassword(password);
        }
        else
        {
            if (channel->isInviteOnly())
            {
                if (!channel->isInvited(client))
                {
                    if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", ERR_INVITEONLYCHAN)))
                        return (false);
                    continue ;
                }
            }
            else
            {
                if (channel->hasPassword())
                {
                    if (password.empty() || !channel->checkPassword(password))
                    {
                        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", ERR_BADCHANNELKEY)))
                            return (false);
                        continue ;
                    }
                }
                
                if (channel->getUserLimit() != -1 && channel->getNbrUsers() >= channel->getUserLimit())
                {
                    if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", ERR_CHANNELISFULL)))
                        return (false);
                    continue ;
                }
            }

            channel->addClient(client);
            channel->removeInvited(client);
        }

        this->broadcastMessage(getMessage(client, NULL, channel, "JOIN", RAW_JOIN), channel);

        if (!channel->getTopic().empty())
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", RPL_TOPIC)))
                return (false);
        }
        else
        {
            if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", RPL_NOTOPIC)))
                return (false);
        }

        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", RPL_NAMREPLY)))
            return (false);
        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", RPL_ENDOFNAMES)))
            return (false);
        if (!this->_socket.Send(fd, getMessage(client, NULL, channel, "JOIN", RPL_CHANNELMODEIS)))
            return (false);
    }
    return (true);
}

