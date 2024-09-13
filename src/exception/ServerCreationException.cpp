#include "../../include/exception/ServerCreationException.hpp"

const char* ServerCreationException::what() const throw()
{
    return ("Failed to create server socket. Check your system resources!");
}