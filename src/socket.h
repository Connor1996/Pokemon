#ifndef SOCKET_H
#define SOCKET_H

//#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <vector>
#include <string>

#include <thread>
#include <mutex>
#include <chrono>


#pragma comment(lib, "Ws2_32.lib")

namespace Connor_Socket
{

#define DEFAULT_BUFLEN 10000        // Socket传输缓冲区大小

#define CLIENT_PORT 5150            // 客户端连接端口
#define SERVER_PORT 2222            // 服务器监听端口

                                    // 单机模式下都指向localhost
#define CLIENT_ADDR "127.0.0.1"     // 客户端IP地址
#define SERVER_ADDR "127.0.0.1"     // 服务器IP地址

using std::cout;
using std::endl;
using std::string;

class Socket{
public:
    Socket()
    {
        // WSAStartup初始化WS2_32.dll
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw std::runtime_error("WSAStartup failed");
    }

    ~Socket()
    {
        // 释放WSA资源
        WSACleanup();
    }

protected:

    // WSA数据信息
    WSADATA wsaData;

};

}

#endif /* SOCKET_H */
