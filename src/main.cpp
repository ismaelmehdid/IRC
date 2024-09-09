#include "../include/irc.hpp"

int main(int argc, char **argv)
{
    parsing(argc, argv);

    Socket *ircserv = new Socket(argv[2]);
    if (!ircserv)
        return 1;

    if (start_server(ircserv, argv) != 0)
        return 1;

    return 0;
}
