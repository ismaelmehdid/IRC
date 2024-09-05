#include "../include/irc.hpp"

int main(int argc, char **argv)
{
    (void)argv;
    if (argc != ARGUMENTS_REQUIRED) {
        display_error_message(ERR_BAD_ARGUMENTS);
        irc_exit(EXIT_FAILURE);
    }
    return 0;
}
