#include <iostream>
#include "server.h"


int main(int argc, char *argv[])
{
    std::cout << "this is server" << std::endl;
    try
    {
        Connor_Socket::Server server;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
