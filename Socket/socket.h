#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define CLIENT_PORT 5150
#define SERVER_PORT 2222

#define CLIENT_ADDR "127.0.0.1"
#define SERVER_ADDR "127.0.0.1"

#pragma comment(lib, "Ws2_32.lib")

namespace Socket
{
using std::cout;
using std::endl;

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
        WSACleanup();
    }
protected:
    WSADATA wsaData;
};


class Client : Socket{
public:
    Client() : Client(CLIENT_PORT)
    { }

    Client(int port) : _port(port)
    {
        _connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_connectSocket == -1)
            throw std::runtime_error("Cannot create socket");

        // 填写客户端地址信息
        // 使用inet_addr需要将IP地址转换为网络格式
        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(_port);
        //_serverAddr.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
        auto res = inet_pton(AF_INET, CLIENT_ADDR, &_serverAddr.sin_addr);
        if (res < 0)
        {
            closesocket(_connectSocket);
            throw std::runtime_error("First parameter is not a valid address family");
        }
        else if (res == 0)
        {
            closesocket(_connectSocket);
            throw std::runtime_error("Second parrameter does not contain valid ipaddress");
        }
        // 向服务器发出连接请求
        if (connect(_connectSocket, reinterpret_cast<SOCKADDR *>(&_serverAddr),
                    sizeof(_serverAddr))== -1)
        {
             closesocket(_connectSocket);
             throw std::runtime_error("Unable to connect to server");
        }

        send();
    }

    ~Client()
    {
        shutdown(_connectSocket, SHUT_RDWR);
        closesocket(_connectSocket);
    }

protected:
    SOCKET _connectSocket;
    SOCKADDR_IN _serverAddr;
    int _port;

    void send()
    {
        #define DEFAULT_BUFLEN 200
        int recvbuflen = DEFAULT_BUFLEN;
        char *sendbuf = "this is a test";
        char recvbuf[DEFAULT_BUFLEN];

        int iResult = ::send(_connectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR)
        {
            closesocketsocket(_connectSocket);
            throw std::runtime_error("Failed at send message");
        }
    }
};

class Server : Socket
{
public:
    Server()
    {
        listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(SERVER_PORT);
        _serverAddr.sin_addr.s_addr = INADDR_ANY;

        // 绑定监听端口
        if (bind(listeningSocket, static_cast<SOCKADDR*>(&_serverAddr), sizeof(_serverAddr)) == -1)
        {
            closesocket(listeningSocket);
            throw std::runtime_error("Failed at bind");
        }

        // 开始监听，指定监听队列的大小
        if (listen(listeningSocket, 5))
        {
            closesocket(listeningSocket);
            throw std::runtime_error("Failed at listen");
        }

        // 不断等待客户端请求的到来
        while (1)
        {
            SOCKET newConnection = accept(listeningSocket, NULL, NULL);
            if (newConnection < 0)
            {
                cout << "Failed at accept" << endl;
                continue;
            }
            char recvBuf[200];
            recv(newConnection, recvBuf, 200, 0);
        }
    }

    ~Server()
    {
        shutdown(listeningSocket, SHUT_RDWR);
        closesocket(listeningSocket);
    }

protected:
    SOCKET listeningSocket;

    SOCKADDR_IN _serverAddr;
    SOCKADDR_IN clientAddr;
};
}
