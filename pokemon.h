#ifndef POKEMON_H
#define POKEMON_H

// 获得对象运行时多态类型

#ifdef __GNUC__
#define GET_CLASS_TYPE(_OBJECT_)     \
    string(abi::__cxa_demangle(typeid(_OBJECT_).name(), nullptr, nullptr, nullptr))
#else
#define GET_CLASS_TYPE(_OBJECT_)     \
    string(typeid(_OBJECT_).name())
#endif


#include <iostream>
#include <ctime>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

enum class Type
{
    Strength,   //高攻击
    Tanker,     //高生命值
    Defensive,  //高防御
    Swift,      //低攻击间隔
};

typedef struct
{
    Type type;
    unsigned int attackPoint;
    unsigned int defensePoint;
    unsigned int healthPoint;
    unsigned int attackFrequence;
}Attribute;

const unsigned int LEVEL_EXP_LIST[15] = {0, 100, 250, 500, 800, 1200, 1800, 2500,
                                         3300, 4500, 6000, 7000, 8000, 9000, 10000};

class Pokemon
{
public:
    Pokemon(string name, unsigned int level, unsigned int exp,
            Attribute attribute)
        : _name(name), _level(level), _exp(exp),
          _attribute(attribute), _hp(attribute.healthPoint)
    {}

    ~Pokemon() {}

    Type GetType() const { return _attribute.type; }
    string GetName() const { return _name; }
    unsigned int GetLevel() const { return _level; }
    unsigned long GetExp() const { return _exp; }
    unsigned int GetAttackPoint() const { return _attribute.attackPoint; }
    unsigned int GetHealthPoint() const { return _attribute.healthPoint; }
    unsigned int GetDefensePoint() const { return _attribute.defensePoint; }
    unsigned int GetAttackFrequence() const { return _attribute.attackFrequence; }
    unsigned int GetHp() const { return _hp; }

    // 根据受到的伤害更新血量，同时返回是否死亡
    virtual bool Hurt(unsigned int damage);

    // 产生一个攻击值
    virtual unsigned int Attack(Pokemon * opPokemon) = 0;

    // 根据获得的经验增加经验值并自动升级，返回是否升级
    virtual bool Upgrade(unsigned int exp);

protected:
    string _name;
    unsigned int _level;
    unsigned long _exp;

    Attribute _attribute;

    unsigned int _hp;

    // 小精灵升级时各属性的成长
    virtual void Grow(unsigned int *);

    // 在攻击、防御、升级时有一定的随机提升，此函数产生随机的比例系数
    virtual double Bonus();

};


class Fire : public Pokemon
{
public:
    Fire(string name, unsigned int level, unsigned int exp,
         Attribute attribute)
        : Pokemon(name, level, exp, attribute)
    {}

    unsigned int Attack(Pokemon * opPokemon) override
    {
        string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water")
            coefficient = 1.1;
        else if (className == "Grass" || className == "Ice")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attribute.attackPoint * coefficient);
    }
};

class Water : public Pokemon
{
    Water(string name, unsigned int level, unsigned int exp,
          Attribute attribute)
         : Pokemon(name, level, exp, attribute)
    {}

    unsigned int Attack(Pokemon * opPokemon) override
    {
        string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Water" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Fire")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attribute.attackPoint * coefficient);
    }
};

class Electricity : public Pokemon
{
public:
    Electricity(string name, unsigned int level, unsigned int exp,
                Attribute attribute)
               : Pokemon(name, level, exp, attribute)
    {}

    unsigned int Attack(Pokemon * opPokemon) override
    {
        string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Electricity" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attribute.attackPoint * coefficient);
    }
};

class Grass : public Pokemon
{
public:
    Grass(string name, unsigned int level, unsigned int exp,
          Attribute attribute)
         : Pokemon(name, level, exp, attribute)
    {}

    unsigned int Attack(Pokemon * opPokemon) override
    {
        string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attribute.attackPoint * coefficient);
    }
};

class Ice : public Pokemon
{
public:
    Ice(string name, unsigned int level, unsigned int exp,
        Attribute attribute)
       : Pokemon(name, level, exp, attribute)
    {}

    unsigned int Attack(Pokemon * opPokemon) override
    {
        string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water" || className == "Ice")
            coefficient = 1.1;
        else if (className == "Grass")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attribute.attackPoint * coefficient);
    }
};

#endif // POKEMON_H
