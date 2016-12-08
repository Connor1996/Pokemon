#ifndef DISPATCH_H
#define DISPATCH_H

#include "server.h"
#include "include/json.hpp"

using json = nlohmann::json;

class Dispatcher
{
public:
    Dispatcher() : _state(0) { }
    Dispatcher(SOCKET &connection, Connor_Socket::Server *parent)
        : _connection(connection), _parent(parent), _state(0)
    { }

    // 根据请求信息，分发到相应的函数处理请求
    std::string Dispatch(json requestInfo);

    json LoginHandle(json&);
    json SignupHandle(json&);
    json OnlineListHandle(json &);
    json UserBagHandle(json &);

    void Logout();
private:
    int _state;

    // 与dispatcher绑定的socket连接
    SOCKET _connection;

    // 与socket连接绑定的用户名
    std::string _username;

    // server指针，以访问server维护的在线列表
    Connor_Socket::Server *_parent;

};

#endif // DISPATCH_H
