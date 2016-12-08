#include "server.h"

#include "dispatch.h"
#include "include/json.hpp"


using namespace Connor_Socket;
using json = nlohmann::json;


Server::Server() : _count(0), _mapper("data.db")
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
            Dispatcher dispatcher(connection, this);

            if (connection < 0)
            {
                cout << "[ERROR] Failed at accept" << endl;
            }
            else
            {
                cout << "[INFO] New come, now " << ++_count << " connections in total" << endl;

                while(true)
                {
                    if (recv(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        // 保证cout完整执行而不被其他线程打断
                        mtx.lock();
                        dispatcher.Logout();
                        cout << "[INFO] Someone offline, now " << --_count << " connections in total" << endl;
                        mtx.unlock();

                        shutdown(connection, SD_BOTH);
                        closesocket(connection);
                        break;
                    }
                    cout << "[INFO] receive message: " << recvBuf << endl;

                    std::string responseStr;
                    try
                    {
                        responseStr = dispatcher.Dispatch(std::move(json::parse(recvBuf)));
                    }
                    catch (std::exception e)
                    {
                        shutdown(connection, SD_BOTH);
                        closesocket(connection);
                        cout << "[ERROR] " << e.what() << endl;
                    }

                    cout << "[INFO] send message: " << responseStr << endl;
                    send(connection, responseStr.c_str(), DEFAULT_BUFLEN, 0);

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

bool Server::Online(std::string username, SOCKET connection)
{
    // emplace返回一个pair，第二个元素为是否成功插入
    // 若map中已经有一个同插入相同的key，则不进行插入
    auto result = _sockets.emplace(std::make_pair(username, connection));
    return result.second;
}

void Server::Offline(std::string username)
{
    _sockets.erase(username);
}

std::list<std::string> Server::GetOnlineList()
{
    std::list<std::string> onlineList;
    for (const auto& pair : _sockets)
        onlineList.push_back(pair.first);

    return std::move(onlineList);
}
