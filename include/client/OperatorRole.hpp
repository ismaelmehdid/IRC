#pragma once

# include "IRole.hpp"

class OperatorRole : public IRole
{
    public:
        OperatorRole();
        OperatorRole(const OperatorRole& other);
        OperatorRole&   operator=(const OperatorRole& other);
        virtual         ~OperatorRole();

        virtual void    kick();
        virtual void    invite();
        virtual void    topic();
        virtual void    mode(char arg);
};