#pragma once

# include "IRole.hpp"

class OperatorRole : public IRole
{
    public:
        OperatorRole();
        OperatorRole(const OperatorRole& other);
        OperatorRole&   operator=(const OperatorRole& other);
        virtual         ~OperatorRole();

        void    kick    (const t_IRCCommand &);
        void    invite  (const t_IRCCommand &);
        void    topic   (const t_IRCCommand &);
        void    pass    (const t_IRCCommand &);
        void    nick    (const t_IRCCommand &);
        void    user    (const t_IRCCommand &);
        void    mode    (const t_IRCCommand &);

        OperatorRole* clone() const;
};