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

#ifdef __SERVER__
    ORMAP(PokemonList, name, type, attackPoint, defensePoint,
          healthPoint, attackFrequence, property);
#endif

};


#endif // POKEMONLIST_H
