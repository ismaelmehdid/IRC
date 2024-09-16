#pragma once

#include "../irc.hpp"

struct t_IRCCommand;

class ARole
{
    public:
        ARole();
        ARole(Client *);
        virtual ~ARole();

        // commands that require operator rights
        virtual void    kick    (const t_IRCCommand &) = 0;
        virtual void    invite  (const t_IRCCommand &) = 0;
        virtual void    topic   (const t_IRCCommand &) = 0;
        virtual void    mode    (const t_IRCCommand &) = 0;

        // commands without operator rights
        void            join    (const t_IRCCommand &command);
        void            part    (const t_IRCCommand &command);
        void            privMsg (const t_IRCCommand &command);
        void            ping    (const t_IRCCommand &command);

        void            pass    (const t_IRCCommand &);
        void            user    (const t_IRCCommand &);
        void            nick    (const t_IRCCommand &);
        void            quit    (const t_IRCCommand &);

        virtual ARole*  clone() const = 0;
    
    protected:
        Client  *_client;
};