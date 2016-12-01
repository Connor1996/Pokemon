#include "client.h"
#include "include/json.hpp"
#include "../define.h"

using namespace Connor_Socket;
using json = nlohmann::json;

Client::Client(string name) : _username(name)
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

bool Client::Connect(std::string password)
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

    json SendInfo = {
        {"type", LOG_IN},
        {"username", _username},
        {"password", password}
    };

    // 获得返回信息，并将其解析成json
    json receiveInfo = json::parse(Request(SendInfo.dump()));
    cout << "receive: " << receiveInfo << endl;

    if (receiveInfo["type"].get<int>() == LOG_IN_FAIL)
    {
        closesocket(_connectSocket);
        return false;
    }
    else if (receiveInfo["type"].get<int>() == LOG_IN_SUCCESS)
    {
        return true;
    }
    else if (receiveInfo["type"].get<int>() == SERVER_ERROR)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Server occurs fatal error");
    }
    else
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Wrong return value for request");
    }
}


std::string Client::Request(string sendBuf)
{
    int recvBufLen = DEFAULT_BUFLEN;
    char recvBuf[DEFAULT_BUFLEN];


    if (send(_connectSocket, sendBuf.c_str(), sendBuf.length(), 0) == SOCKET_ERROR)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Failed at send message");
    }
    cout << "send complete" << endl;
    if (recv(_connectSocket, recvBuf, recvBufLen, 0) <= 0)
    {
        closesocket(_connectSocket);
        throw std::runtime_error("Failed at receive message");
    }

    return std::move(std::string(recvBuf));
}
