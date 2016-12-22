#ifndef USERBAG_H
#define USERBAG_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif


struct UserBag
{
    // 该小精灵在数据库中的独特id号
    ORMLite::Nullable<int> id;

    // 小精灵各种属性
    std::string name;
    int level;
    int exp;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;
    std::string property;

    // 持有该小精灵的用户名
    std::string username;

#ifdef __SERVER__
    ORMAP(UserBag, id, name, level, exp, type, attackPoint, defensePoint,
          healthPoint, attackFrequence, property, username);
#endif

};

#endif // USERBAG_H
