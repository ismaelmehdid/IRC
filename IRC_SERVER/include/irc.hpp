#pragma once

//===----------------------------------------------------------------------===//

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sstream>
#include <cctype> 
#include <algorithm>
#include <sys/socket.h>  // socket, bind, listen, accept, setsockopt, send, recv
#include <netinet/in.h>  // htons, htonl, ntohs, ntohl
#include <arpa/inet.h>   // inet_addr, inet_ntoa
#include <netdb.h>       // getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
#include <unistd.h>      // close, lseek
#include <fcntl.h>       // fcntl
#include <poll.h>        // poll
#include <csignal>       // signal, sigaction
#include <sys/types.h>   // types for socket(), accept()
#include <sys/stat.h>    // fstat
#include <cerrno>        // errno
#include <vector>
#include <set>
#include <exception>
#include <iomanip>       // setfill/setw...

#include "server/serverMessages.hpp"

//===----------------------------------------------------------------------===//

#define MIN_PORT_VALUE 1024
#define MAX_PORT_VALUE 65535

#define ARGUMENTS_REQUIRED  3

#define MAX_NICKNAME_LENGTH 20
#define MAX_CHANNEL_NAME_LENGTH 50
#define MAX_TOPIC_LENGTH 300

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 25
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define BG_BLUE "\033[44m"
#define WHITE   "\033[37m"

//===----------------------------------------------------------------------===//

class Socket;
class Server;
class Client;
class Channel;

//===----------------------------------------------------------------------===//

enum t_errors
{
    ERR_BAD_ARGUMENTS,
    ERR_BAD_PORT
};

struct t_IRCCommand
{
    std::string                 prefix;
    std::string                 command;
    std::vector<std::string>    params;
    std::string                 trailing;
};

struct t_ModeCommandData
{
    Client                      *client;
    int                         client_fd;
    Channel                     *channelToModify;
    std::string                 modesToChange;
    size_t                      parameter_index;
    Client                      *target_to_change;
    bool                        addMode;
    t_IRCCommand                command;
};

//===----------------------------------------------------------------------===//

extern Server *global_ircserv;

//===----------------------------------------------------------------------===//

void                        validateArguments(int argc, char **argv);
void                        display_error_message(t_errors code);
std::vector<t_IRCCommand>   parseRequests(const std::string &requests);
void                        handleShuttingDown(int sig);
void                        displayUserMessage(const std::string &nickName, const std::string &message);

//===----------------------------------------------------------------------===//