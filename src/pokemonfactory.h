#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include "pokemon.h"
#include <algorithm>
#include <cctype>

class PokemonFactory
{
public:
    PokemonFactory() {}

    Pokemon* Create(string name);

private:
    Attribute QueryInDateBase(string name, string &property);
};

#endif // POKEMONFACTORY_H
