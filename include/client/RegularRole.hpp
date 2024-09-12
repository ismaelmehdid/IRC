#pragma once

# include "IRole.hpp"

class RegularRole : public IRole
{
    public:
        RegularRole();
        RegularRole(const RegularRole& other);
        RegularRole&    operator=(const RegularRole& other);
        virtual         ~RegularRole();

        void    kick    (const t_IRCCommand &);
        void    invite  (const t_IRCCommand &);
        void    topic   (const t_IRCCommand &);
        void    pass    (const t_IRCCommand &);
        void    nick    (const t_IRCCommand &);
        void    user    (const t_IRCCommand &);
        void    mode    (const t_IRCCommand &);

        RegularRole* clone() const;
};