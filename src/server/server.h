#ifndef SERVER_H
#define SERVER_H

#include "../socket.h"

namespace Connor_Socket {

class Server : Socket
{
public:
    Server();

    ~Server();

protected:
    // 监听客户端访问的socket
    SOCKET _listeningSocket;

    // 地址信息
    SOCKADDR_IN _serverAddr;
    SOCKADDR_IN _clientAddr;

    // 持有与各个客户端保持通信的线程
    std::vector<std::thread> _socketThreads;

    // 连接到服务器的客户端数
    size_t _count;
};

}
#endif // SERVER_H
