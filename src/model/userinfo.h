#ifndef USERINFO_H
#define USERINFO_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif


struct UserInfo
{
    std::string username;
    std::string password;

#ifdef __SERVER__
    ORMAP(UserInfo, username, password);
#endif

};

#endif // USERINFO_H
