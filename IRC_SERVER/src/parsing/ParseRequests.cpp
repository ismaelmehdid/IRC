#include "../../include/irc.hpp"

/**
 * @brief Parses a single IRC request from a raw request string.
 *
 * This function takes a raw IRC request string and parses it into a structured
 * t_IRCCommand object. The raw request string is expected to follow the IRC
 * protocol format.
 *
 * @param rawRequest The raw IRC request string to be parsed.
 * @return A t_IRCCommand object containing the parsed request details.
 *
 * The parsing process involves:
 * - Extracting the prefix if it exists (indicated by a leading ':').
 * - Extracting the command.
 * - Extracting the parameters and trailing part of the request.
 *
 * The t_IRCCommand object will have the following fields populated:
 * - prefix: The prefix of the request (if any).
 * - command: The command of the request.
 * - params: A vector of parameters.
 * - trailing: The trailing part of the request (if any).
 */
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

    size_t  space_pos = rawRequest.find(' ', pos);
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
 * @brief Parses a string containing multiple IRC requests separated by "\r\n".
 *
 * This function takes a string of concatenated IRC requests, each separated by
 * the "\r\n" delimiter, and parses them into individual t_IRCCommand objects.
 * Each parsed command is then stored in a vector which is returned.
 *
 * @param requests A string containing multiple IRC requests separated by "\r\n".
 * @return A vector of t_IRCCommand objects, each representing a parsed IRC request.
 */
std::vector<t_IRCCommand>   parseRequests(const std::string &requests)
{
    std::vector<t_IRCCommand>   extracted;
    size_t                      start = 0;
    size_t                      end;

    while ((end = requests.find("\r\n", start)) != std::string::npos)
    {
        std::string             request = requests.substr(start, end - start);
        t_IRCCommand            parsedRequest = parseSingleRequest(request);

        extracted.push_back(parsedRequest);
        start = end + 2;
    }

    return (extracted);
}
