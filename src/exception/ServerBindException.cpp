#include "../../include/exception/ServerBindException.hpp"

const char* ServerBindException::what() const throw()
{
    return ("Server binding failed. The port might be already in use.");
}