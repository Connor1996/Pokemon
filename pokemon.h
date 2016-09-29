#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <ctime>
#include <typeinfo>

using namespace std;

enum class Type
{
    Strength,   //高攻击
    Tanker,     //高生命值
    Defensive,  //高防御
    Swift,      //低攻击间隔
};

const unsigned int LEVEL_EXP_LIST[15] = {0, 100, 250, 500, 800, 1200, 1800, 2500,
                                         3300, 4500, 6000, 7000, 8000, 9000, 10000};

class Pokemon
{
public:
    Pokemon(Type type, string name, unsigned int level, unsigned int exp,
            unsigned int attackPoint, unsigned int defensePoint, unsigned int healthPoint,
            unsigned int attackFrequence)
        : _type(type), _name(name), _level(level), _exp(exp), _attackPoint(attackPoint),
          _defensePoint(defensePoint), _healthPoint(healthPoint),
          _attackFrequence(attackFrequence), _hp(healthPoint)
    {}

    ~Pokemon() {}

    Type GetType() const { return _type; }
    string GetName() const { return _name; }
    unsigned int GetLevel() const { return _level; }
    unsigned long GetExp() const { return _exp; }
    unsigned int GetAttackPoint() const { return _attackPoint; }
    unsigned int GetHealthPoint() const { return _healthPoint; }
    unsigned int GetDefensePoint() const { return _defensePoint; }
    unsigned int GetAttackFrequence() const { return _attackFrequence; }

    // 根据受到的伤害更新血量，同时返回是否死亡
    virtual bool Hurt(unsigned int damage);

    // 产生一个攻击值
    virtual unsigned int Attack(Pokemon & opPokemon) = 0;

    // 根据获得的经验增加经验值并自动升级，返回是否升级
    virtual bool Upgrade(unsigned int exp);

protected:
    Type _type;
    string _name;
    unsigned int _level;
    unsigned long _exp;

    unsigned int _attackPoint;
    unsigned int _defensePoint;
    unsigned int _healthPoint;
    unsigned int _attackFrequence;

    int _hp;

    // 小精灵升级时各属性的成长
    virtual void Grow(unsigned int *);

    // 在攻击、防御、升级时有一定的随机提升，此函数产生随机的比例系数
    virtual double Bonus();

};


class Fire : public Pokemon
{
public:
    unsigned int Attack(Pokemon & opPokemon) override
    {
        string className = typeid(opPokemon).name();
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water")
            coefficient = 1.1;
        else if (className == "Grass" || className == "Ice")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attackPoint * coefficient);
    }
};

class Water : public Pokemon
{
    unsigned int Attack(Pokemon & opPokemon) override
    {
        string className = typeid(opPokemon).name();
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Water" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Fire")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attackPoint * coefficient);
    }
};

class Electricity : public Pokemon
{
public:
    unsigned int Attack(Pokemon & opPokemon) override
    {
        string className = typeid(opPokemon).name();
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Electricity" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attackPoint * coefficient);
    }
};

class Grass : public Pokemon
{
public:
    unsigned int Attack(Pokemon & opPokemon) override
    {
        string className = typeid(opPokemon).name();
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attackPoint * coefficient);
    }
};

class Ice : public Pokemon
{
public:
    unsigned int Attack(Pokemon & opPokemon) override
    {
        string className = typeid(opPokemon).name();
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water" || className == "Ice")
            coefficient = 1.1;
        else if (className == "Grass")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() > 0.94)
            coefficient += 1;
        return static_cast<unsigned int>(_attackPoint * coefficient);
    }
};

#endif // POKEMON_H
