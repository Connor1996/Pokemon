#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include <string>

class Pokemon;
typedef struct struct_Attribute Attribute;

#include "client/client.h"
using Client = Connor_Socket::Client;

class PokemonFactory
{
public:
    PokemonFactory() {}

    static Pokemon* CreateComputer(std::string name, Connor_Socket::Client *client);
    static Pokemon* CreateUser(std::string str);

};

#endif // POKEMONFACTORY_H
