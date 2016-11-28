#include "server.h"

using namespace Connor_Socket;

Server::Server() : _count(0)
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

    std::mutex mtx; //各个连接客户端线程输出信息锁
    while (true)
    {
        SOCKET newConnection = accept(_listeningSocket, NULL, NULL);
        _socketThreads.emplace_back(std::thread([this, &mtx] (SOCKET &&connection)
        {
            char recvBuf[DEFAULT_BUFLEN];
            int recvBufLen = DEFAULT_BUFLEN;

            if (connection < 0)
            {
                cout << "Failed at accept" << endl;
            }
            else
            {
                cout << "New come, now " << ++_count << " connections in total" << endl;

                while(true)
                {
                    if (recv(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        // 保证cout完整执行而不被其他线程打断
                        mtx.lock();
                        cout << "Someone offline, now " << --_count << " connections in total" << endl;
                        mtx.unlock();

                        shutdown(connection, SD_BOTH);
                        closesocket(connection);
                        break;
                    }
                    cout << "message: " << recvBuf << endl;

                    char sendBuf[DEFAULT_BUFLEN] = "1";
                    int sendBufLen = DEFAULT_BUFLEN;

                    send(connection, sendBuf, sendBufLen, 0);
                }
            }
        }, std::move(newConnection)));

        _socketThreads.back().detach(); //使子线程独立运行
    }
}

Server::~Server()
{
    shutdown(_listeningSocket, SD_BOTH);
    closesocket(_listeningSocket);
}
