#pragma once

#include "../irc.hpp"

struct t_IRCCommand;
// Role Interface for operator and regular users
class ARole
{
    public:
        ARole();
        ARole(Client *);
        virtual ~ARole();

        virtual void    kick    (const t_IRCCommand &) = 0;
        virtual void    invite  (const t_IRCCommand &) = 0;
        virtual void    topic   (const t_IRCCommand &) = 0;
        virtual void    mode    (const t_IRCCommand &) = 0;

        void    pass    (const t_IRCCommand &);
        void    user    (const t_IRCCommand &);
        void    nick    (const t_IRCCommand &);
        void    quit    (const t_IRCCommand &);

        virtual ARole*  clone() const = 0;
    
    protected:
        Client *_client;
};