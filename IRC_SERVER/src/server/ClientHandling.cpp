#include "../../include/server/Server.hpp"

void    Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
}

void    Server::removeClient(Client* user, std::string reason)
{
    std::list<std::string>  empty_channels;
    int                     fd = user->get_fd();

//--remove user from every channel
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        Channel* channel = it->second;

        if (channel->isMember(user))
        {
            channel->removeClient(user);

            // Check if the channel is empty and should be deleted
            if (channel->getNbrUsers() == 0)
                empty_channels.push_back(channel->getName());

            this->broadcastMessage(user->getPrefix() + " QUIT :" + reason, channel);
        }
    }

//--delete empty channels
    for (std::list<std::string>::const_iterator it = empty_channels.begin(); it != empty_channels.end(); ++it)
    {
        delete (this->_channels[*it]);
        this->_channels.erase(*it);
    }

//--delete user from poll struct
    for (int i = 0; i < this->_poll_count; ++i)
    {
        if (this->_fds[i].fd == fd)
        {
            pollRemove(i); //removing data from the poll struct
            break ;
        }
    }

//--delete user from Server
    
    this->_clients.erase(fd);
    this->_nbr_clients--;
    std::vector<std::string>::iterator it = std::find(this->_nicknames.begin(), this->_nicknames.end(), user->getNickName());
    
    if (it != this->_nicknames.end())
    {
        this->_nicknames.erase(it);
    }
    
    delete user;
}

void    Server::pollRemove(int index)
{
    if (index >= 0 && index < this->_poll_count)
    {
        close(this->_fds[index].fd); //closing the fd of the user

        if (index != this->_poll_count - 1)
        {
            std::swap(this->_fds[index], this->_fds[this->_poll_count - 1]);
        }
        --this->_poll_count;
        this->_fds.pop_back();
    }
}