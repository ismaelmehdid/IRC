#include "../../include/server/Server.hpp"

void    Server::addClient(Client *client) 
{
    this->_clients[client->get_fd()] = client;
    this->_nbr_clients++;
    for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++) {
        std::cout << it->fd << " ";
    }
    std::cout << std::endl;
}

void    Server::removeClient(Client* user, const std::string &reason)
{
    int fd = user->get_fd();

    removeClientFromEveryChannels(user, reason);

    this->_clients.erase(fd);
    this->_nbr_clients--;
    std::vector<std::string>::iterator it = std::find(this->_nicknames.begin(), this->_nicknames.end(), user->getNickName());
    if (it != this->_nicknames.end()) {
        this->_nicknames.erase(it);
    }

    std::cout << fd << std::endl;


    delete user;
}

void    Server::removeClientFromEveryChannels(Client* user, const std::string &reason)
{
    std::map<std::string, Channel*>::iterator it = _channels.begin();

    while (it != _channels.end()) {
        if (it->second) {
            if (it->second->removeClient(user) == true) { // check if the user has indead been removed from this channel
                this->broadcastMessage(user->getPrefix() + " QUIT :" + reason, it->second); // quit message for the users inside the channel the user have been removed to
                if (it->second->getNbrUsers() == 0) { // if the channel is now empty we have to delete it
                    delete it->second; // delete the channel
                    std::map<std::string, Channel*>::iterator toErase = it;
                    ++it; // increment the iterator before erasing
                    _channels.erase(toErase); // erase the channel from the channel map of the server                   
                    continue; // we skiped the erased element above so we don't need to increment 'it'
                }
            }
        }
        ++it;
    }
}
