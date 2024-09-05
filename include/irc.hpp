#pragma once

#include <iostream>
#include <cstdlib>

#define ARGUMENTS_REQUIRED 3

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

enum t_errors {
    ERR_BAD_ARGUMENTS
};

void display_error_message(t_errors code);
void irc_exit(int exit_code);