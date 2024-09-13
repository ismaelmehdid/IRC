#include "../../include/exception/ServerListenException.hpp"

const char* ServerListenException::what() const throw()
{
    return ("Server failed to start listening. Check your network settings.");
}