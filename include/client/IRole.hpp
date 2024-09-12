#pragma once

#include "../irc.hpp"

struct t_IRCCommand;
// Role Interface for operator and regular users
class IRole
{
    public:
        virtual void    kick    (const t_IRCCommand &) = 0;
        virtual void    invite  (const t_IRCCommand &) = 0;
        virtual void    topic   (const t_IRCCommand &) = 0;
        virtual void    pass    (const t_IRCCommand &) = 0;
        virtual void    user    (const t_IRCCommand &) = 0;
        virtual void    nick    (const t_IRCCommand &) = 0;
        virtual void    mode    (const t_IRCCommand &) = 0;

        virtual IRole*  clone() const = 0;
        virtual         ~IRole() {};
};