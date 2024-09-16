#pragma once

#include "../client/Client.hpp"

class Channel
{
    private:
        const std::string       _name;
        std::string             _topic;
        std::map<int, Client*>  _clients;

    public:
        Channel(const std::string& name);
        ~Channel();

        void                            addClient(Client* client);
        void                            removeClient(Client* client);
        void                            broadcastMessage(const std::string& message);
        void                            setTopic(const std::string& topic);
        const std::string&              getName() const;
        const std::string&              getTopic() const;
        const std::map<int, Client*>&   getClients() const;
        bool                            isMember(Client* client) const;
        
};
