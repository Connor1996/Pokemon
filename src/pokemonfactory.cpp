﻿#include "pokemonfactory.h"

#include "pokemon.h"
#include "define.h"
#include "include/json.hpp"
using json = nlohmann::json;

#include <algorithm>
#include <cctype>

Pokemon* PokemonFactory::CreateComputer(std::string name, Client* client)
{
    auto queryInDateBase = [&](std::string name) -> std::pair<std::string, Attribute>
    {
        transform(name.begin(), name.end(), name.begin(), ::tolower);

        json sendInfo = {
            {"type", GET_POKEMON_INFO},
            {"name", name}
        };
        json receiveInfo = json::parse(client->Send(sendInfo.dump()));
        if (receiveInfo["type"].get<int>() == SERVER_ERROR)
            throw std::runtime_error("Failed at create computer pokemon");

        json info = json::parse(receiveInfo["info"].get<std::string>());
        Attribute attr = {
            Type(std::stoi(info["type"].get<std::string>())),
            std::stoi(info["attackPoint"].get<std::string>()),
            std::stoi(info["defensePoint"].get<std::string>()),
            std::stoi(info["healthPoint"].get<std::string>()),
            std::stoi(info["attackFrequence"].get<std::string>())
        };

        return std::move(std::make_pair(info["property"].get<std::string>(), attr));
    };

    auto info = queryInDateBase(name);
    // 获取创建相应种类的回调函数
    PTRCreateObject callback = Reflector::GetInstance().GetClassByName(info.first);
    if (callback == NULL)
        throw std::runtime_error("wrong pokemon property");

    return callback(name, 1, 0, info.second);

}

Pokemon* PokemonFactory::CreateUser(std::string str)
{
    // 先反序列化属性字符串
    //str.push_back(EOF);
    std::stringstream ss(str);
    std::string line;
    json info;

    while(std::getline(ss, line))
    {
        auto pos = line.find(" : ");
        auto key = line.substr(0, pos);
        auto value = line.substr(pos + 3);

        if (key == "property" || key == "name")
            info[key] = value;
        else
            info[key] = std::stoi(value);
    }

    Attribute attr = {
        Type(info["type"].get<int>()),
        info["attackPoint"].get<int>(),
        info["defensePoint"].get<int>(),
        info["healthPoint"].get<int>(),
        info["attackFrequence"].get<int>()
    };
    auto type = info["property"].get<std::string>();
    // 获取创建相应种类的回调函数
    PTRCreateObject callback = Reflector::GetInstance()
            .GetClassByName(type);

    return callback(info["name"].get<std::string>(),
            info["level"].get<int>(), info["exp"].get<int>(), attr);

}

