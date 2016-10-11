#include "pokemonfactory.h"
#include "pokemon.h"
#include <algorithm>
#include <cctype>

Pokemon* PokemonFactory::Create(std::string name)
{
    std::string property = "";

    Attribute info = QueryInDateBase(name, property);
    // 获取创建相应种类的回调函数
    PTRCreateObject callback = Reflector::GetInstance().GetClassByName(property);

    return callback(name, 1, 0, info);

}

Attribute PokemonFactory::QueryInDateBase(std::string name, std::string &property)
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
