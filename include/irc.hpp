#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sstream>
#include <cctype> 
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
#include <exception>

#define MIN_PORT_VALUE 1024
#define MAX_PORT_VALUE 65535

#define ARGUMENTS_REQUIRED  3
#define MAX_CLIENTS_IN_QUEU 5

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define MSG_WELCOME             ":server 001 client :Welcome to the IRC server!\r\n"
#define ERR_PASSWORD_REQUIRED   ":server 461 * PASS :Password required\r\n"
#define ERR_PASSWORD_INCORRECT  ":server 464 * PASS :Password incorrect\r\n"
#define ERR_NO_NICKNAME_GIVEN   ":server 431 * :No nickname given\r\n"
#define ERR_NEED_MORE_PARAMS    ":server 461 * :Not enough parameters\r\n"
#define ERR_ALREADY_REGISTERED  ":server 462 * :You may not reregister\r\n"
#define ERR_NICKNAME_IN_USE     ":server 433 * NICK :Nickname already used by another user\r\n"
#define ERR_UNKNOWNCOMMAND      ":server 421 * :Unknown command\r\n"

class Socket;
class Server;
class Client;
class ARole;
class OperatorRole; // inherit from IRole
class RegularRole;  // inherit from IRole

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

extern Server *global_ircserv;

// Parsing
void                        display_error_message(t_errors code);
void                        irc_exit(int exit_code);
void                        parsing(int argc, char **argv);
std::vector<t_IRCCommand>   parse_client_commands(const std::string &commands);
