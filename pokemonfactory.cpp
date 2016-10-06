#include "pokemonfactory.h"

Pokemon* PokemonFactory::Create(string name)
{
    string property = "";

    Attribute info = QueryInDateBase(name, property);
    // 后期实现反射机制，减少代码量
    if (property == "Fire")
        return new Fire(name, 1, 0, info);
    else if (property == "Electricity")
        return new Electricity(name, 1, 0, info);
}

Attribute PokemonFactory::QueryInDateBase(string name, string &property)
{
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    // 硬编码化，后期加入数据库时更新
    Attribute attr;
    if (name == "pikachu")
    {
        attr = {Type::Swift, 20, 20, 20, 20};
        property = "Electricity";
    }
    else if (name == "charmander")
    {
        attr = {Type::Strength, 20, 20, 20, 20};
        property = "Fire";
    }

    return attr;
}
