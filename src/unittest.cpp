#include <iostream>
#include "include/catch.hpp"
#include "pokemon.h"
#include "pokemonfactory.h"
#include "model.h"
#include <iostream>


TEST_CASE("Test Pokemon Class")
{
    PokemonFactory factory = PokemonFactory();
    Pokemon* pikachu = factory.Create("Pikachu");
    Pokemon* charmander = factory.Create("Charmander");

    REQUIRE(pikachu->GetName() == "Pikachu");
    REQUIRE(charmander->GetName() == "Charmander");
    REQUIRE(GET_CLASS_TYPE(*pikachu) == "Electricity");

    // 测试攻击函数和被伤害函数
    SECTION("one attacks the other one")
    {
        auto damage = pikachu->Attack(charmander);
        auto result = charmander->Hurt(damage);
        if (result == false)
            REQUIRE(charmander->GetHp() == charmander->GetHealthPoint());
        else
            REQUIRE(charmander->GetHp() < charmander->GetHealthPoint());
    }

    // 测试获得经验但未升级情况
    SECTION("get experience but not upgrade")
    {
        auto result = pikachu->Upgrade(10);
        REQUIRE(pikachu->GetExp() == 10);
        REQUIRE(result == false);
        REQUIRE(pikachu->GetLevel() == 1);
    }

    // 测试获得经验并升级情况
    SECTION("get experience and upgrade")
    {
        auto result = pikachu->Upgrade(1000);
        REQUIRE(pikachu->GetExp() == 1000);
        REQUIRE(result == true);
        REQUIRE(pikachu->GetLevel() >= 1);
    }

    delete pikachu;
    delete charmander;
}



TEST_CASE("Test ORMLite")
{
    ORMapper<MyClass> mapper("test.db");

    REQUIRE(mapper.CreateTable() == true);

    std::vector<MyClass> objects =
    {
        {3, 0.2, "John"},
        {4, 0.4, "Jack"},
        {5, 0.6, "Jess"}
    };

    for (const auto obj : objects)
    {
        REQUIRE(mapper.Insert(obj));
    }
//    SECTION("insert objects")
//    {
//        for (const auto obj : objects)
//        {
//            REQUIRE(mapper.Insert(obj));
//        }

//    }

    SECTION("update object")
    {
        objects[1].score = 1.0;
        REQUIRE(mapper.Update (objects[1]));
    }

    SECTION("delete object")
    {
        REQUIRE(mapper.Delete (objects[2]));
    }

    SECTION("select all to vector")
    {
        QueryMessager<MyClass> query;
        mapper.Query(query.Where(Field(MyClass{}.id) == 3));
        auto vec = query.GetVector();
        for (auto v : vec)
        {
            for (auto str : v)
                std::cout << str << " ";
            std::cout << std::endl;
        }
    }

    mapper.DropTable();
}
