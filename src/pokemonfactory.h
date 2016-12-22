#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include <string>

class Pokemon;
typedef struct struct_Attribute Attribute;

#include "client/client.h"
using Client = Connor_Socket::Client;

// 产生小精灵的工厂
// 通过多态获得小精灵对象
class PokemonFactory
{
public:
    PokemonFactory() {}

    // 产生对战电脑的小精灵实体
    // @param:
    //      name 对战小精灵的名字
    //      client 与服务器连接的socket指针
    // @return:
    //      小精灵对象指针
    static Pokemon* CreateComputer(std::string name, Connor_Socket::Client *client);

    // 产生用户的小精灵实体
    // @param:
    //      str 用户小精灵的序列化数据，包含各种属性
    // @return:
    //      小精灵对象指针
    static Pokemon* CreateUser(std::string str);

};

#endif // POKEMONFACTORY_H
