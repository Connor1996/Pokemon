#include <iostream>
#include <./socket.h>


int main(int argc, char *argv[])
{
    std::cout << "this is client" << std::endl;
    std::string name = (argc == 1) ? "UNKNOWN" : argv[1];
    try
    {
        Socket::Client client = Socket::Client(name);
    } catch (std::exception e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}

