#pragma once

// Role Interface for operator and regular users

# include <iostream>

class IRole
{
    public:
        virtual void    kick() = 0;
        virtual void    invite() = 0;
        virtual void    topic() = 0;
        virtual void    mode(char arg) = 0;
        virtual         ~IRole() {};
};