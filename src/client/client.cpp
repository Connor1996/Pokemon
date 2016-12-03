#include "client.h"
#include "include/json.hpp"
#include "../define.h"

using namespace Connor_Socket;

Client::Client(string name)
    : _username(name)
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
    cout << "Initial success" << endl;
}

Client::~Client()
{
    shutdown(_connectSocket, SD_BOTH);
    closesocket(_connectSocket);
}

void Client::Close()
{
    closesocket(_connectSocket);
}

std::string Client::Connect(std::string requestInfo)
{
    auto trys = 5;
    auto success = false;

    while(trys--)
    {
        if (connect(_connectSocket, reinterpret_cast<SOCKADDR *>(&_serverAddr),
                    sizeof(_serverAddr))== -1)
        {
             cout << "Unable to connect to server... " << std::to_string(5 - trys) << " trys" << endl;
             std::this_thread::sleep_for(std::chrono::milliseconds(10));
             continue;
        }
        cout << "Connect success" << endl;
        success = true;
        break;
    }
    if (!success)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Unable to connect to server, Please try later");
    }

    return std::move(Send(requestInfo));
}

std::string Client::Send(std::string requestInfo)
{

    int recvBufLen = DEFAULT_BUFLEN;
    char recvBuf[DEFAULT_BUFLEN];

    cout << "[INFO] request: " << requestInfo << endl;
    if (send(_connectSocket, requestInfo.c_str(), DEFAULT_BUFLEN, 0) == SOCKET_ERROR)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Failed at send message");
    }
    //cout << "send complete" << endl;
    if (recv(_connectSocket, recvBuf, recvBufLen, 0) <= 0)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Failed at receive message");
    }

    cout << "receive: " << recvBuf << endl;

    return std::move(std::string(recvBuf));
}


