#ifndef POKEMONINFO_H
#define POKEMONINFO_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif

struct PokemonInfo
{
    //小精灵各种属性
    int id;
    std::string name;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;

    std::string property;

#ifdef __SERVER__
    ORMAP(PokemonInfo, id, name, type, attackPoint, defensePoint,
          healthPoint, attackFrequence, property);
#endif

};

#endif // POKEMONINFO_H
