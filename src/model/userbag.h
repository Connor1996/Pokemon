#ifndef USERBAG_H
#define USERBAG_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif


struct UserBag
{
    ORMLite::Nullable<int> id;
    //小精灵属性
    std::string name;
    int level;
    int exp;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;
    std::string property;

    std::string username;

#ifdef __SERVER__
    ORMAP(UserBag, id, name, level, exp, type, attackPoint, defensePoint,
          healthPoint, attackFrequence, property, username);
#endif

};

#endif // USERBAG_H
