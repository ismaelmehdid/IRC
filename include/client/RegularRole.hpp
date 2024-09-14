#pragma once

# include "ARole.hpp"

class RegularRole : public ARole
{
    public:
        RegularRole(Client *);
        RegularRole(const RegularRole& other);
        RegularRole&    operator=(const RegularRole& other);
        virtual         ~RegularRole();

        void    kick    (const t_IRCCommand &);
        void    invite  (const t_IRCCommand &);
        void    topic   (const t_IRCCommand &);
        void    mode    (const t_IRCCommand &);

        RegularRole* clone() const;

    private:
        RegularRole();
};