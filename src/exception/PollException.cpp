#include "../../include/exception/PollException.hpp"

const char* PollException::what() const throw()
{
    return ("Polling failed. Unable to monitor file descriptors.");
}