#ifndef POKEMONINFO_H
#define POKEMONINFO_H

#ifdef __SERVER__
#include "../server/ormlite.h"

#endif

struct PokemonInfo
{
    //小精灵属性
    int id;
    std::string name;

    int type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;

#ifdef __SERVER__
    ORMAP(PokemonInfo, id, name, type, attackPoint, defensePoint,
          healthPoint, attackFrequence);
#endif

};

#endif // POKEMONINFO_H
