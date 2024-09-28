#pragma once

# include "../irc.hpp"

//===----------------------------------------------------------------------===//

class ServerListenException : public std::exception
{
    public:
        virtual const char* what() const throw();
};

//===----------------------------------------------------------------------===//