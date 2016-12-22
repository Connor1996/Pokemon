#ifndef DISPATCH_H
#define DISPATCH_H

#include "server.h"
#include "include/json.hpp"

using json = nlohmann::json;

class Dispatcher
{
public:

    Dispatcher() : _state(0) { }

    // 传入SOCKET和Server的构造函数
    // @param:
    //      connection  与相应客户端建立的socket连接
    //      parent      server对象指针
    Dispatcher(SOCKET &connection, Connor_Socket::Server *parent)
        : _connection(connection), _parent(parent), _state(0)
    {
        // 初始化随机种子
        srand((unsigned int)time(NULL));
    }

    // 根据请求信息，分发到相应的函数处理请求
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    std::string Dispatch(json requestInfo);

    // 登陆处理逻辑
    json LoginHandle(json&);
    // 注册处理逻辑
    json SignupHandle(json&);
    // 获取在线用户列表处理逻辑
    json OnlineListHandle(json &);
    // 查看用户背包处理逻辑
    json UserBagHandle(json &);
    // 获取离线用户列表处理逻辑
    json OfflineListHandle(json &);
    // 获取用户成就逻辑
    json UserAchievementHandle(json &);
    // 获取对战精灵列表处理逻辑
    json PokemonListHandle(json &);
    // 获取对战精灵信息处理逻辑
    json PokemonInfoHandle(json &);
    // 对战胜利处理逻辑
    json GameWinHandle(json &);
    // 对战失败处理逻辑
    json GameLoseHandle(json &);
    // 损失小精灵处理逻辑
    json LosePokemonHandle(json &);

    // 为用户随机分发一个宠物小精灵
    // @param:
    //      username 获得小精灵的用户名
    // @return:
    //      获得小精灵的名字
    std::string DispatchPokemon(std::string username);

    // 与该Dispatch绑定用户登出
    void Logout();

    // 获取Dispatcher的内部状态
    int getState() { return _state; }
private:
    // 代表用户处于什么状态，如在线空闲或对战
    int _state;

    // 与dispatcher绑定的socket连接
    SOCKET _connection;

    // 与socket连接绑定的用户名
    std::string _username;

    // server指针，以访问server维护的在线列表
    Connor_Socket::Server *_parent;

};

#endif // DISPATCH_H
