#pragma once

#include "../client/Client.hpp"
#include <map>
#include <set>

class Channel
{
    private:
        const std::string       _name;
        std::string             _topic;

        std::set<Client *>      _clients;
        std::set<Client *>      _operators;   // For channel operators
        std::set<Client *>      _invited;     // For invited clients

        bool                    _inviteOnly;  // Invite-only mode flag
        bool                    _topicLocked; // Restrict topic change to operators
        std::string             _password;    // Channel password (optional)
        int                     _userLimit;   // Max number of users allowed (-1 for no limit)

    public:
        Channel(const std::string& name);
        ~Channel();

//------Getters
        const std::string&              getName() const;
        const std::string&              getTopic() const;
        int                             getUserLimit() const;
        const std::set<Client*>&        getClients() const;
        bool                            isMember(Client* client) const;
        bool                            isOperator(Client* client) const;
        bool                            isInvited(Client* client) const;
        bool                            isInviteOnly() const;
        bool                            hasPassword() const;
        bool                            checkPassword(const std::string& key) const;

//------Setters
        void                            setTopic(const std::string& topic);
        void                            setTopicLocked(bool mode); // t mode
        void                            setInviteOnly(bool mode);  // i mode
        void                            setPassword(const std::string& key); // k mode
        void                            setUserLimit(int limit);   // l mode

//------Client management
        void                            addClient(Client* client);
        void                            removeClient(Client* client);
        void                            addOperator(Client* client);
        void                            removeOperator(Client* client);
        //void                          addInvited(Client* client); // idk if its needed
        //void                          removeInvited(Client* client); // idk if its needed
};
