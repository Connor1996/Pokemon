#ifndef POKEMONLIST_H
#define POKEMONLIST_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif

struct PokemonList
{
    //小精灵属性
    std::string name;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;

    std::string property;

    // 击败小精灵可以获得的经验
    int exp;

#ifdef __SERVER__
    ORMAP(PokemonList, name, type, attackPoint, defensePoint,
          healthPoint, attackFrequence, property, exp);
#endif

};


#endif // POKEMONLIST_H
