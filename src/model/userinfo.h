#ifndef USERINFO_H
#define USERINFO_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif


struct UserInfo
{
    // 用户名
    std::string username;
    // 密码
    std::string password;
    // 胜利局数
    int win;
    // 失败局数
    int lose;
    // 宠物个数
    int sum;
    // 高级宠物个数
    int advanceSum;

#ifdef __SERVER__
    ORMAP(UserInfo, username, password, win, lose, sum, advanceSum);
#endif

};

#endif // USERINFO_H
