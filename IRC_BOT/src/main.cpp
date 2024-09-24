#include "../include/smartboi.hpp"
#include "../include/SmartBoi/SmartBoi.hpp"

int main(int argc, char **argv)
{
    if (argc != EXPECTED_NBR_OF_ARGS) {
        std::cerr << "Wrong number of arguments: ./smartboi ip port password" << std::endl;
        return 1;
    }

    try {
        SmartBoi smartboi;
        smartboi.start(argv[1], argv[2], argv[3]);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}