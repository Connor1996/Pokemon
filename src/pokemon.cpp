﻿#include "pokemon.h"
#include <ctime>
#include <typeinfo>

// 注册类到反射MAP
REGISTER(Fire);
REGISTER(Water);
REGISTER(Electricity);
REGISTER(Grass);
REGISTER(Ice);


bool Pokemon::Hurt(int damage)
{
    // 闪避
    if (Bonus() < MISS_RATE)
        return false;

    auto score = (0.85 + Bonus() / 4) * (damage - _attribute.defensePoint);
    if (score > 0)
        if (_hp <= score)
        {
            _hp = 0;
            return true;
        }
        else
            _hp -= score;
    return false;
}

bool Pokemon::Upgrade(int exp)
{
    // 增加经验值，达到15级后无法继续升级，故直接返回false
    _exp += exp;
    if (_level >= 15)
        return false;

    // 循环判断当前经验值是否达到升级条件，以适应一次升多级的可能情况
    auto isUpgraded = false;
    while (_exp >= LEVEL_EXP_LIST[_level])
    {
        _level++;

        int *master = NULL;
        switch (GetType()) {
        case Type::Strength:
            master = &_attribute.attackPoint;
            break;
        case Type::Tanker:
            master = &_attribute.healthPoint;
            break;
        case Type::Defensive:
            master = &_attribute.defensePoint;
            break;
        case Type::Swift:
            master = &_attribute.attackPoint;
            break;
        default:
            break;
        }
        Grow(master);
        isUpgraded = true;
    }
    return isUpgraded;
}

#include <iostream>
#define N 999
double Pokemon::Bonus()
{
    //随机生成0-1的小数.
    return static_cast<double>(rand() % (N + 1)) / (N + 1);
}

void Pokemon::Grow(int * master)
{
    int * values[4] = {&_attribute.attackPoint, &_attribute.defensePoint,
                                &_attribute.healthPoint, &_attribute.attackPoint};

    //遍历变量指针，与master相同的说明为主要属性，升级时增加的幅度更大
    for (auto ptr : values)
    {
        if (ptr == master)
            *ptr = *ptr + static_cast<int>((*ptr / 2 + _level) * Bonus() / 2);
        else
            *ptr = *ptr + static_cast<int>((*ptr / 2 + _level / 2) * Bonus() / 3);
    }
}
