#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include <string>

class Pokemon;
typedef struct struct_Attribute Attribute;

class PokemonFactory
{
public:
    PokemonFactory() {}

    Pokemon* Create(std::string name);

private:
    Attribute QueryInDateBase(std::string name, std::string &property);
};

#endif // POKEMONFACTORY_H
