#include <iostream>
#include <./socket.h>


int main(int argc, char *argv[])
{
    std::cout << "this is client" << std::endl;
    try{
        Socket::Client client;
    } catch (std::exception e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}

