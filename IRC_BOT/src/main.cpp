#include "../include/smartboi.hpp"
#include "../include/SmartBoi/SmartBoi.hpp"

SmartBoi *global_smartboi = NULL;

void handleShuttingDown(int signal_code) {
    delete global_smartboi;
    std::exit(signal_code);
}

int main(int argc, char **argv)
{
    if (argc != EXPECTED_NBR_OF_ARGS)
    {
        std::cerr << "Wrong number of arguments: ./smartboi ip port password" << std::endl;
        return (1);
    }

    try
    {
        signal(SIGINT, handleShuttingDown);
        signal(SIGQUIT, handleShuttingDown);
        global_smartboi = new SmartBoi();
        global_smartboi->start(argv[1], argv[2], argv[3]);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        delete global_smartboi;
        return (1);
    }
    return (0);
}