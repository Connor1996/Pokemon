#include <iostream>
#include "../socket.h"


int main(int argc, char *argv[])
{
    std::cout << "this is server" << std::endl;
    try
    {
        Socket::Server server;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
