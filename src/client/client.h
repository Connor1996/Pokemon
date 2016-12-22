#ifndef CLIENT_H
#define CLIENT_H

#include "../socket.h"

namespace Connor_Socket {

class Client : Socket{
public:
    // 默认构造函数
    // 委托到带名字的构造函数，传参为UNKOWN
    Client() : Client("UNKNOWN") { }

    // 带名字的构造函数
    // @param:
    //      name 用户名
    Client(string name);

    ~Client();

    // 注册、登陆时使用的函数
    // 建立与服务器的连接
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    std::string Connect(std::string requestInfo);

    // 建立连接后使用此函数向服务器发送数据
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    std::string Send(std::string requestInfo);


    // 建立与服务器的连接
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    void Close();

    // 获取该client绑定的用户名
    // @return:
    //      用户名
    std::string GetUserName() { return _username; }

protected:
    // 与服务器通信的socket
    SOCKET _connectSocket;

    // 服务器的地址信息
    SOCKADDR_IN _serverAddr;

    // 用户名
    string _username;
};

}
#endif // CLIENT_H
