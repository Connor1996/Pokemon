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
    Server();

    ~Server();

    bool Online(std::string username, SOCKET connection);

    void Offline(std::string username);

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

    // 链接数据库的mapper对象
    ORMLite::ORMapper<UserInfo> _mapper;
};

}
#endif // SERVER_H
