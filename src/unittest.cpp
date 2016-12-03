#include <iostream>
#include "include/catch.hpp"
#include "pokemon.h"
#include "pokemonfactory.h"
#include "model.h"
#include "server/ormlite.h"
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

    SECTION("simulate a fight")
    {
        auto speed_p = pikachu->GetAttackFrequence();
        auto speed_c = charmander->GetAttackFrequence();
        std::cout << "[INFO] the speed of " << pikachu->GetName() << " is " << speed_p;
        std::cout << "[INFO] the speed of " << charmander->GetName() << " is " << speed_c;
        std::cout << "[INFO] Fight begin" << std::endl;
        // 时间轴
        size_t time_p = 0;
        size_t time_c = 0;

        while (time_p++, time_c++)
        {
            if (time_p == speed_p)
            {
                std::cout << "[INFO] " << pikachu->GetName() <<
                             "attack" << charmander->GetName() << std::endl;
                auto damage = pikachu->Attack(charmander);
                std::cout << "[INFO] " << charmander->GetName() << " get hurt by "
                          << damage << std::endl;
                if (charmander->Hurt(damage))
                {
                    std::cout << "[INFO] " << charmander->GetName() << " is died" << std::endl;
                    break;
                }
                time_p = 0;
            }

            if (time_c == speed_c)
            {
                std::cout << "[INFO] " << charmander->GetName() <<
                             "attack" << pikachu->GetName() << std::endl;
                auto damage = charmander->Attack(pikachu);
                std::cout << "[INFO] " << pikachu->GetName() << " get hurt by "
                          << damage << std::endl;
                if (pikachu->Hurt(damage))
                {
                    std::cout << "[INFO] " << pikachu->GetName() << " is died" << std::endl;
                    break;
                }
                time_c = 0;
            }

        }
        std:: cout << "[INFO] Fight over" << std::endl;
    }

    delete pikachu;
    delete charmander;
}



TEST_CASE("Test ORMLite")
{
    ORMapper<MyClass> mapper("test.db");

    std::vector<MyClass> objects =
    {
        {3, 0.2, "John"},
        {4, 0.4, "Jack"},
        {5, 0.6, "Jess"}
    };

    for (const auto obj : objects)
    {
        if (mapper.Insert(obj) == false)
            std::cout << mapper.GetErrorMessage() << std::endl;
    }

//    objects[1].score = 1.1;
//    REQUIRE(mapper.Update(objects[1]));

//    QueryMessager<MyClass> query;
//    mapper.Query(query);
//    auto vec = query.GetVector();
//    for (auto item : vec)
//        for (auto str : item)
//                     std::cout << str << " ";
//                   std::cout << std::endl;

//    REQUIRE(query.IsNone() == false);

    SECTION("update object")
    {



    }

//    SECTION("delete object")
//    {
//        REQUIRE(mapper.Delete(objects[2]));
//    }

    SECTION("select all to vector")
    {
        MyClass helper;
        QueryMessager<MyClass> query(helper);
        mapper.Query(query.Where(Field(helper.id) == 3 && Field(helper.name) == "John"));
        auto vec = query.GetVector();
        for (auto v : vec)
        {
            for (auto str : v)
                std::cout << str << " ";
            std::cout << std::endl;
        }
    }

   REQUIRE(mapper.DropTable());

}
