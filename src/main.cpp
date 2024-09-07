#include "../include/irc.hpp"

int main(int argc, char **argv)
{
    Socket *ircserv = new Socket();
    if (!ircserv)
        return 1;

    parsing(argc, argv);

    if (start_server(ircserv, argv) != 0)
        return 1;

    return 0;
}
