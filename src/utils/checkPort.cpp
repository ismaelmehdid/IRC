#include "../../include/irc.hpp"

bool    checkPort(const char *port)
{
    if (!port || *port == '\0')
        return (false);

    size_t  len = std::strlen(port);

    if (len < 4 || len > 5)
        return (false);

    for (size_t i = 0; i < len; ++i)
    {
        if (!std::isdigit(port[i]))
            return (false);
    }

    char        *endptr;
    long int    portCheck = std::strtol(port, &endptr, 10);

    if (*endptr != '\0' || portCheck < 1024 || portCheck > 65535)
        return (false);

    return (true);
}