#pragma once

# include "../irc.hpp"

//===----------------------------------------------------------------------===//

class ServerBindException : public std::exception
{
    public:
        virtual const char* what() const throw();
};

//===----------------------------------------------------------------------===//