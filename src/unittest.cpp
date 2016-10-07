#include "include/catch.hpp"
#include "pokemon.h"
#include "pokemonfactory.h"

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
}
