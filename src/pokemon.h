#ifndef POKEMON_H
#define POKEMON_H

#include "reflector.h"
#include <ctime>

// 获得对象运行时多态类型
// 多编译器支持
#ifdef __GNUC__
#include <cxxabi.h>
#define GET_CLASS_TYPE(_OBJECT_)     \
    std::string(abi::__cxa_demangle(typeid(_OBJECT_).name(), nullptr, nullptr, nullptr))
#elif _MSC_VER && !__INTEL_COMPILER
#include <typeinfo>

#define GET_CLASS_NAME(_OBJECT_) \
    std::string(typeid(_OBJECT_).name())
#define GET_CLASS_TYPE(_OBJECT_)     \
    GET_CLASS_NAME(_OBJECT_).substr(GET_CLASS_NAME(_OBJECT_).find("class ") + 6, \
    GET_CLASS_NAME(_OBJECT_).length() - 6)

#else
#define GET_CLASS_TYPE(_OBJECT_) \
    std::string(typeid(_OBJECT_).name())

#endif

enum class Type
{
    Strength,   //高攻击
    Tanker,     //高生命值
    Defensive,  //高防御
    Swift,      //低攻击间隔
};


typedef struct struct_Attribute
{
    Type type;
    int attackPoint;
    int defensePoint;
    int healthPoint;
    int attackFrequence;
}Attribute;


const int LEVEL_EXP_LIST[15] = {0, 100, 250, 500, 800, 1200, 1800, 2500,
                                         3300, 4500, 6000, 7000, 8000, 9000, 10000};

class Pokemon
{
public:
    Pokemon(std::string name, int level, int exp,
            Attribute attribute)
        : _name(name), _level(level), _exp(exp),
          _attribute(attribute), _hp(attribute.healthPoint)
    {
        srand(time(NULL));
    }

    ~Pokemon() {}

    Type GetType() const { return _attribute.type; }
    std::string GetName() const { return _name; }
    int GetLevel() const { return _level; }
    unsigned long GetExp() const { return _exp; }
    int GetAttackPoint() const { return _attribute.attackPoint; }
    int GetHealthPoint() const { return _attribute.healthPoint; }
    int GetDefensePoint() const { return _attribute.defensePoint; }
    int GetAttackFrequence() const { return _attribute.attackFrequence; }
    int GetHp() const { return _hp; }

    // 根据受到的伤害更新血量，同时返回是否死亡
    virtual bool Hurt(int damage);

    // 产生一个攻击值
    virtual int Attack(Pokemon * opPokemon) = 0;

    // 根据获得的经验增加经验值并自动升级，返回是否升级
    virtual bool Upgrade(int exp);

protected:
    std::string _name;
    int _level;
    unsigned long _exp;

    Attribute _attribute;

    int _hp;

    // 小精灵升级时各属性的成长
    virtual void Grow(int *);

    // 在攻击、防御、升级时有一定的随机提升，此函数产生随机的比例系数
    virtual double Bonus();

};


class Fire : public Pokemon
{
public:
    Fire(std::string name, int level, int exp,
         Attribute attribute)
        : Pokemon(name, level, exp, attribute)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water")
            coefficient = 1.1;
        else if (className == "Grass" || className == "Ice")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.8)
            coefficient += 1;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Water : public Pokemon
{
public:
    Water(std::string name, int level, int exp,
          Attribute attribute)
         : Pokemon(name, level, exp, attribute)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Water" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Fire")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.8)
            coefficient += 1;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Electricity : public Pokemon
{
public:
    Electricity(std::string name, int level, int exp,
                Attribute attribute)
               : Pokemon(name, level, exp, attribute)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Electricity" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.8)
            coefficient += 1;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Grass : public Pokemon
{
public:
    Grass(std::string name, int level, int exp,
          Attribute attribute)
         : Pokemon(name, level, exp, attribute)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.8)
            coefficient += 1;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Ice : public Pokemon
{
public:
    Ice(std::string name, int level, int exp,
        Attribute attribute)
       : Pokemon(name, level, exp, attribute)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        std::string name = typeid(*opPokemon).name() ;
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water" || className == "Ice")
            coefficient = 1.1;
        else if (className == "Grass")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.8)
            coefficient += 1;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};



#endif // POKEMON_H
