#include "../../include/server/Server.hpp"

void    Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
}

void Server::removeClient(Client* user, std::string reason)
{
    std::list<std::string> empty_channels;
    int fd = user->get_fd();

//--remove user from every channel
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        Channel* channel = it->second;

        if (channel->isInvited(user))
        {
            channel->removeInvited(user);
        }
        if (channel->isMember(user))
        {
            // Remove user from the channel
            channel->removeClient(user);

            // Remove user from operators and invited lists if they exist there
            if (channel->isOperator(user))
            {
                channel->removeOperator(user);
            }

            // Check if the channel is empty and should be deleted
            if (channel->getClients().empty())
            {
                empty_channels.push_back(channel->getName());
            }

            // Broadcast the QUIT message to all remaining clients in the channel
            for (std::set<Client*>::const_iterator iter = channel->getClients().begin(); iter != channel->getClients().end(); ++iter)
            {
                this->_socket.send((*iter)->get_fd(), user->getPrefix() + " QUIT :" + reason);
            }
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
            pollRemove(i);
            break;
        }
    }

//--delete user from Server
    delete (this->_clients[fd]);
    this->_clients.erase(fd);
    this->_nbr_clients--;
}


void    Server::pollRemove(int index)
{
    close (this->_fds[index].fd);

    if (index != this->_poll_count - 1)
    {
        this->_fds[index].fd = this->_fds[this->_poll_count - 1].fd;
        this->_fds[index].events = this->_fds[this->_poll_count - 1].events;
    }

    --this->_poll_count;
    
    this->_fds.pop_back();
}