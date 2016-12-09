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


#ifdef __SERVER__
    ORMAP(UserInfo, username, password);
#endif

};

#endif // USERINFO_H
