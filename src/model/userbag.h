#ifndef USERBAG_H
#define USERBAG_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif


struct UserBag
{
    std::string username;
    //小精灵属性
    std::string name;
    int level;
    int exp;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;

#ifdef __SERVER__
    ORMAP(UserBag, username, name, level, exp, type, attackPoint, defensePoint,
          healthPoint, attackFrequence);
#endif

};

#endif // USERBAG_H
