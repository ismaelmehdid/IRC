#pragma once

#include "server/Socket.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
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

#define ARGUMENTS_REQUIRED 3

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

enum t_errors
{
    ERR_BAD_ARGUMENTS,
    ERR_BAD_PORT
};

// Server
int     start_server(Socket *ircserv, char **argv);

// Parsing
void    display_error_message(t_errors code);
void    irc_exit(int exit_code);
void    parsing(int argc, char **argv);