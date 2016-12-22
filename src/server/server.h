#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <set>

#include "../socket.h"
#include "../model/userinfo.h"

namespace Connor_Socket {

class Server : Socket
{
public:
    // 构造函数，打开监听接口等待请求
    Server();

    ~Server();

    // 查询用户是否在线
    // @param:
    //      username 需要查询的用户名
    //      connection 与该用户名绑定的socket
    // @return:
    //      是否在线
    bool Online(std::string username, SOCKET connection);

    // 将某用户从在线列表移除
    // @param:
    //      username 需要移除的用户名
    void Offline(std::string username);

    // 获得在线用户列表
    // @return:
    //      装载有所有在线用户名的list
    std::list<std::string> GetOnlineList();
protected:
    // 监听客户端访问的socket
    SOCKET _listeningSocket;

    // 地址信息
    SOCKADDR_IN _serverAddr;

    // 持有与各个客户端保持通信的线程
    std::vector<std::thread> _socketThreads;

    // 持有用户名相对应的socket链接
    std::unordered_map<std::string, SOCKET> _sockets;

    // 连接到服务器的客户端数
    size_t _count;
};

}
#endif // SERVER_H
