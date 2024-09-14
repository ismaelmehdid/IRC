#pragma once

# include "ARole.hpp"

class OperatorRole : public ARole
{
    public:
        OperatorRole(Client *);
        OperatorRole(const OperatorRole& other);
        OperatorRole&   operator=(const OperatorRole& other);
        virtual         ~OperatorRole();

        void    kick    (const t_IRCCommand &);
        void    invite  (const t_IRCCommand &);
        void    topic   (const t_IRCCommand &);
        void    mode    (const t_IRCCommand &);

        OperatorRole* clone() const;

    private:
        OperatorRole();
};