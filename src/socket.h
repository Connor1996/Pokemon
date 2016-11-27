#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <vector>
#include <string>

#include <thread>
#include <mutex>
#include <chrono>

#define CLIENT_PORT 5150
#define SERVER_PORT 2222

#define CLIENT_ADDR "127.0.0.1"
#define SERVER_ADDR "127.0.0.1"

#pragma comment(lib, "Ws2_32.lib")

namespace Socket
{
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
        WSACleanup();
    }
protected:
    WSADATA wsaData;
};


class Client : Socket{
public:
    Client() { }
    Client(string name) : _name(name)
    {
        std::cout << "begin..." << std::endl;
        _connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_connectSocket == -1)
            throw std::runtime_error("Cannot create socket");

        // 填写客户端地址信息
        // 使用inet_addr需要将IP地址转换为网络格式
        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(SERVER_PORT);
        //_serverAddr.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
        auto res = inet_pton(AF_INET, SERVER_ADDR, &_serverAddr.sin_addr);
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
        auto trys = 10;
        while(trys--)
        {
            if (connect(_connectSocket, reinterpret_cast<SOCKADDR *>(&_serverAddr),
                        sizeof(_serverAddr))== -1)
            {
                 cout << "Unable to connect to server... " << std::to_string(10 - trys) << " trys" << endl;
                 std::this_thread::sleep_for(std::chrono::milliseconds(100));
                 continue;
            }
            cout << "Initial success" << endl;
            while(true)
            {
                getchar();
                send();
            }
        }
        closesocket(_connectSocket);
        throw std::runtime_error("Unable to connect to server, Please try later");
    }

    ~Client()
    {
        shutdown(_connectSocket, SD_BOTH);
        closesocket(_connectSocket);
    }

protected:
    SOCKET _connectSocket;
    SOCKADDR_IN _serverAddr;
    string _name;

    void send()
    {
        #define DEFAULT_BUFLEN 200
        int recvbuflen = DEFAULT_BUFLEN;
        string sendbuf = "this is a test, by: " + _name;
        char recvbuf[DEFAULT_BUFLEN];

        int iResult = ::send(_connectSocket, sendbuf.c_str(), sendbuf.length(), 0);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(_connectSocket);
            throw std::runtime_error("Failed at send message");
        }
        cout << "send complete" << endl;
    }
};

class Server : Socket
{
public:
    Server()
    {
        _listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_listeningSocket == -1)
            throw std::runtime_error("Cannot create listening socket");

        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_port = htons(SERVER_PORT);
        _serverAddr.sin_addr.s_addr = INADDR_ANY;

        // 绑定监听端口
        if (bind(_listeningSocket, reinterpret_cast<SOCKADDR*>(&_serverAddr),
                 sizeof(_serverAddr)) == -1)
        {
            closesocket(_listeningSocket);
            throw std::runtime_error("Failed at bind");
        }

        // 开始监听，指定监听队列的大小
        if (listen(_listeningSocket, 5))
        {
            closesocket(_listeningSocket);
            throw std::runtime_error("Failed at listen");
        }
        std::cout << "Wait for connection..." << std::endl;
        // 不断等待客户端请求的到来
        while (1)
        {
            SOCKET newConnection = accept(_listeningSocket, NULL, NULL);
            _socketThreads.emplace_back(std::thread([this] (SOCKET &&connection)
            {
                char recvBuf[200];
                if (connection < 0)
                {
                    cout << "Failed at accept" << endl;
                }
                else
                {
                    cout << "New come, now " << _socketThreads.size() << " connections in total" << endl;

                    while(true)
                    {
                        if (recv(connection, recvBuf, 200, 0) == -1)
                        {
                            shutdown(connection, SD_BOTH);
                            closesocket(connection);
                            break;
                        }
                        cout << "message: " << recvBuf << endl;
                    }
                }
            }, std::move(newConnection)));
        }
    }

    ~Server()
    {
        shutdown(_listeningSocket, SD_BOTH);
        closesocket(_listeningSocket);
    }

protected:
    SOCKET _listeningSocket;

    SOCKADDR_IN _serverAddr;
    SOCKADDR_IN _clientAddr;

    std::vector<std::thread> _socketThreads;
};
}
