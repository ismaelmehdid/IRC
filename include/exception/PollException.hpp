#pragma once

# include "../irc.hpp"

class PollException : public std::exception
{
    public:
        virtual const char* what() const throw();
};