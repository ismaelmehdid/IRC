#pragma once

# include "IRole.hpp"

class RegularRole : public IRole
{
    public:
        RegularRole();
        RegularRole(const RegularRole& other);
        RegularRole&    operator=(const RegularRole& other);
        virtual         ~RegularRole();

        virtual void    kick();
        virtual void    invite();
        virtual void    topic();
        virtual void    changeMode(char arg);
};