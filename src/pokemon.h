#ifndef POKEMON_H
#define POKEMON_H

#include "reflector.h"
#include <ctime>

// 获得对象运行时多态类型
// 多编译器支持
#ifdef __GNUC__
#include <cxxabi.h>
#define GET_CLASS_TYPE(_OBJECT_)        \
    std::string(abi::__cxa_demangle(typeid(_OBJECT_).name(), nullptr, nullptr, nullptr))
#elif _MSC_VER && !__INTEL_COMPILER
#include <typeinfo>

#define GET_CLASS_NAME(_OBJECT_)        \
    std::string(typeid(_OBJECT_).name())
#define GET_CLASS_TYPE(_OBJECT_)        \
    GET_CLASS_NAME(_OBJECT_).substr(GET_CLASS_NAME(_OBJECT_).find("class ") + 6, \
    GET_CLASS_NAME(_OBJECT_).length() - 6)

#else
#define GET_CLASS_TYPE(_OBJECT_)        \
    std::string(typeid(_OBJECT_).name())

#endif

#define CRTICAL_RATE 0.1    // 暴击概率
#define MISS_RATE 0.2       // 闪避概率

// 小精灵主属性类别
enum class Type
{
    Strength,               //高攻击
    Tanker,                 //高生命值
    Defensive,              //高防御
    Swift,                  //低攻击间隔
};

// 小精灵属性
typedef struct struct_Attribute
{
    Type type;              // 小精灵主属性类型
    int attackPoint;        // 攻击力
    int defensePoint;       // 防御力
    int healthPoint;        // 生命值
    int attackFrequence;    // 攻击间隔
}Attribute;

// 经验值列表
const int LEVEL_EXP_LIST[15] = {0, 100, 250, 500, 800, 1200, 1800, 2500,
                                3300, 4500, 6000, 7000, 8000, 9000, 10000};

class Pokemon
{
public:
    //
    Pokemon(std::string name, int level, int exp,
            Attribute attribute, int id)
        : _name(name), _level(level), _exp(exp),
          _attribute(attribute), _hp(attribute.healthPoint),
          _id(id), _critical(false)
    {
        // 初始化随机种子
        srand(time(NULL));
    }

    ~Pokemon() {}

    // 各种获取属性函数
    Type GetType() const { return _attribute.type; }
    std::string GetName() const { return _name; }
    int GetLevel() const { return _level; }
    unsigned long GetExp() const { return _exp; }
    int GetAttackPoint() const { return _attribute.attackPoint; }
    int GetHealthPoint() const { return _attribute.healthPoint; }
    int GetDefensePoint() const { return _attribute.defensePoint; }
    int GetAttackFrequence() const { return _attribute.attackFrequence; }
    int GetHp() const { return _hp; }
    int GetId() const { return _id; }

    // 判断是否最近一次攻击为暴击
    bool IsCritical() const { return _critical; }

    // 小精灵受伤函数
    // 小精灵真正受到的伤害 = （damage-小精灵的防御值） * 随机系数
    // 同时有几率闪避
    // @param:
    //      damage 受到的伤害
    // @return:
    //      小精灵是否死亡
    virtual bool Hurt(int damage);

    // 小精灵攻击函数
    // @param:
    //      opPokemon 攻击小精灵的指针
    // @return:
    //      攻击造成的伤害
    virtual int Attack(Pokemon * opPokemon) = 0;

    // 根据获得的经验增加经验值并自动升级，返回是否升级
    // @param:
    //      exp 小精灵获得的经验值
    // @return:
    //      小精灵是否升级
    virtual bool Upgrade(int exp);

protected:
    // 精灵名字
    std::string _name;
    // 等级
    int _level;
    // 经验值
    unsigned long _exp;
    // 各种属性
    Attribute _attribute;
    // 对战时的实时血量
    int _hp;
    // 在数据库中对应的唯一id
    int _id;
    // 暂存最近一次攻击是否暴击
    bool _critical;

    // 小精灵升级时各属性的成长
    virtual void Grow(int *);

    // 在攻击、防御、升级时有一定的随机提升，此函数产生随机的比例系数
    virtual double Bonus();

};


class Fire : public Pokemon
{
public:
    Fire(std::string name, int level, int exp,
         Attribute attribute, int id)
        : Pokemon(name, level, exp, attribute, id)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        // 获得对手小精灵运行时类型
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water")
            coefficient = 1.1;
        else if (className == "Grass" || className == "Ice")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() < CRTICAL_RATE)
        {
            _critical = true;
            coefficient += 1;
        }
        else
            _critical = false;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Water : public Pokemon
{
public:
    Water(std::string name, int level, int exp,
          Attribute attribute, int id)
         : Pokemon(name, level, exp, attribute, id)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        // 不同属性间的效果加成
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Water" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Fire")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() < CRTICAL_RATE)
        {
            _critical = true;
            coefficient += 1;
        }
        else
            _critical = false;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Electricity : public Pokemon
{
public:
    Electricity(std::string name, int level, int exp,
                Attribute attribute, int id)
               : Pokemon(name, level, exp, attribute, id)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        // 不同属性间的效果加成
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Electricity" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() < CRTICAL_RATE)
        {
            _critical = true;
            coefficient += 1;
        }
        else
            _critical = false;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Grass : public Pokemon
{
public:
    Grass(std::string name, int level, int exp,
          Attribute attribute, int id)
         : Pokemon(name, level, exp, attribute, id)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        // 不同属性间的效果加成
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Grass")
            coefficient = 1.1;
        else if (className == "Water")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() < CRTICAL_RATE)
        {
            _critical = true;
            coefficient += 1;
        }
        else
            _critical = false;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};


class Ice : public Pokemon
{
public:
    Ice(std::string name, int level, int exp,
        Attribute attribute, int id)
       : Pokemon(name, level, exp, attribute, id)
    {}

    int Attack(Pokemon * opPokemon) override
    {
        // 不同属性间的效果加成
        std::string name = typeid(*opPokemon).name() ;
        std::string className = GET_CLASS_TYPE(*opPokemon);
        auto coefficient = 1.0;

        // 不同属性间的效果加成
        if (className == "Fire" || className == "Water" || className == "Ice")
            coefficient = 1.1;
        else if (className == "Grass")
            coefficient = 1.3;

        // 有几率产生暴击
        if (Bonus() < CRTICAL_RATE)
        {
            _critical = true;
            coefficient += 1;
        }
        else
            _critical = false;
        return static_cast<int>(_attribute.attackPoint * coefficient);
    }
};

#endif // POKEMON_H
