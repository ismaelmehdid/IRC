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


void Server::join(Client *client, const t_IRCCommand &command)
{
    int fd = client->get_fd();

    if (!client->is_authenticated())
    {
        this->_socket.send(fd, ERR_NOTREGISTERED);
        return;
    }

    if (command.params.empty())
    {
        this->_socket.send(fd, ERR_NEED_MORE_PARAMS);
        return;
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
            this->_socket.send(fd, ERR_NO_SUCH_CHANNEL);
            continue ;
        }

        Channel *channel = this->findChannel(channelName);

        if (channel && channel->isMember(client))
        {
            this->_socket.send(fd, getMessage(client, ALREADY_JOINED_ERROR, channel));
            continue ;
        }

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
                    this->_socket.send(fd, getMessage(client, ERR_MODE_I, channel));
                    continue ;
                }
                else
                    channel->removeInvited(client);
            }

            if (channel->getUserLimit() != -1 && channel->getNbrUsers() >= channel->getUserLimit())
            {
                this->_socket.send(fd, getMessage(client, ERR_MODE_L, channel));
                continue ;
            }

            if (channel->hasPassword())
            {
                if (password.empty() || !channel->checkPassword(password))
                {
                    this->_socket.send(fd, getMessage(client, ERR_MODE_K, channel));
                    continue ;
                }
            }

            channel->addClient(client);
        }

        this->broadcastMessage(getMessage(client, JOIN, channel), channel);

        if (!channel->getTopic().empty())
            this->_socket.send(fd, getMessage(client, TOPIC, channel));
        else
            this->_socket.send(fd, getMessage(client, NO_TOPIC, channel));

        this->_socket.send(fd, getMessage(client, NAMES_REPLY, channel));
        this->_socket.send(fd, getMessage(client, END_OF_NAMES, channel));
        this->_socket.send(fd, getMessage(client, MODE, channel));
    }
}

