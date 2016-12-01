// 引入单元测试库——catch
#ifdef __DEBUG__
#   define CATCH_CONFIG_RUNNER
#   include "include/catch.hpp"
#endif

#include <iostream>
#include "server.h"


int main(int argc, char *argv[])
{
#ifdef __DEBUG__
    Catch::Session().run(argc, argv);
#endif

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
