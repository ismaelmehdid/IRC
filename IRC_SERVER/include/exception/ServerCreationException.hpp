#pragma once

# include "../irc.hpp"

//===----------------------------------------------------------------------===//

class ServerCreationException : public std::exception
{
    public:
        virtual const char* what() const throw();
};

//===----------------------------------------------------------------------===//