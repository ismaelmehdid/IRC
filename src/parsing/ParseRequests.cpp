#include "../../include/irc.hpp"

static t_IRCCommand parseSingleRequest(const std::string &rawRequest)
{
    t_IRCCommand    request;
    size_t          pos = 0;

    if (rawRequest[0] == ':')
    {
        pos = rawRequest.find(' ');
        request.prefix = rawRequest.substr(1, pos - 1);
        pos++;
    }

    size_t space_pos = rawRequest.find(' ', pos);
    request.command = rawRequest.substr(pos, space_pos - pos);
    pos = space_pos + 1;

    while (pos < rawRequest.length())
    {
        if (rawRequest[pos] == ':')
        {
            request.trailing = rawRequest.substr(pos + 1);
            break ;
        }

        space_pos = rawRequest.find(' ', pos);
        if (space_pos == std::string::npos)
        {
            request.params.push_back(rawRequest.substr(pos));
            break ;
        }
        else
        {
            request.params.push_back(rawRequest.substr(pos, space_pos - pos));
            pos = space_pos + 1;
        }
    }

    return (request);
}

/**
 * Parses a string of client commands and extracts individual commands into a vector of t_IRCCommand.
 *
 * @param commands The string of client commands to be parsed.
 * @return A vector of t_IRCCommand containing the extracted commands.
 */
std::vector<t_IRCCommand>   parseRequests(const std::string &requests)
{

    std::vector<t_IRCCommand>   extracted;
    size_t                      start = 0;
    size_t                      end;

    while ((end = requests.find("\r\n", start)) != std::string::npos)
    {
        std::string     request         = requests.substr(start, end - start);
        t_IRCCommand    parsedRequest   = parseSingleRequest(request);

        extracted.push_back(parsedRequest);
        start = end + 2;
    }

    return (extracted);
}
