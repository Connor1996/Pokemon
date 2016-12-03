#ifndef CLIENT_H
#define CLIENT_H

#include "../socket.h"

namespace Connor_Socket {

class Client : Socket{
public:
    Client() : Client("UNKNOWN") { }

    Client(string name);

    ~Client();

    // 连接到服务器，验证密码
    // @param:
    //      password 用户输入的密码
    // @return:
    //      连接是否成功
    std::string Connect(std::string requestInfo);

    std::string Send(std::string requestInfo);

    void Close();

protected:
    // 与服务器通信的socket
    SOCKET _connectSocket;

    // 服务器的地址信息
    SOCKADDR_IN _serverAddr;

    // 用户名
    string _username;

    // 向服务器发送请求
    // @param:
    //      sendBuf 需要发送的内容
    // @return:
    //      从服务端传回的响应信息
    //std::string Request(string sendBuf);

};

}
#endif // CLIENT_H
