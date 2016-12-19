#include "dispatch.h"
#include "ormlite.h"

#include "../define.h"
#include "../model/userinfo.h"
#include "../model/userbag.h"
#include "../model/pokemoninfo.h"
#include "../model/pokemonlist.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>

using namespace ORMLite;

// 数据库中小精灵的种类数
#define N 4

std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;
    //std::cout << "[INFO] dispatch" << std::endl;
    switch (requestInfo["define"].get<int>()) {
    case LOG_IN:
        responseInfo = LoginHandle(requestInfo);
        break;
    case SIGN_UP:
        responseInfo = SignupHandle(requestInfo);
        break;
    case GET_ONLINE_LIST:
        responseInfo = OnlineListHandle(requestInfo);
        break;
    case GET_USER_BAG:
        responseInfo = UserBagHandle(requestInfo);
        break;
    case GET_OFFLINE_LIST:
        responseInfo = OfflineListHandle(requestInfo);
        break;
    case GET_USER_ACH:
        responseInfo = UserAchievementHandle(requestInfo);
        break;
    case GET_POKEMON_LIST:
        responseInfo = PokemonListHandle(requestInfo);
        break;
    case GET_POKEMON_INFO:
        responseInfo = PokemonInfoHandle(requestInfo);
        break;
    case GAME_WIN:
        responseInfo = GameWinHandle(requestInfo);
        break;
    case GAME_LOSE:
        responseInfo = GameLoseHandle(requestInfo);
        break;
    case LOSE_POKEMON:
        responseInfo = LosePokemonHandle(requestInfo);
        break;
    default:
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] Bad request" << std::endl;
        break;
    }

    return std::move(responseInfo.dump());
}

json Dispatcher::LoginHandle(json &requestInfo)
{
    ORMapper<UserInfo> mapper("data.db");
    UserInfo helper;
    QueryMessager<UserInfo> messager(helper);
    json responseInfo;

    std::cout << "[INFO] Login request comes" << std::endl;

    auto result = mapper.Query(messager
                 .Where(Field(helper.username) == requestInfo["username"].get<std::string>()
                        && Field(helper.password) == requestInfo["password"].get<std::string>()));
    //std::cout << "[INFO] Query finish" << std::endl;

    if (result)
    {
        if (messager.IsNone())
            responseInfo["define"] = LOG_IN_FAIL_WP;
        else
        {
            // 将username加入在线列表
            _username = requestInfo["username"].get<std::string>();
            if (_parent->Online(_username, _connection))
                responseInfo["define"] = LOG_IN_SUCCESS;
            else
                responseInfo["define"] = LOG_IN_FAIL_AO;
        }
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::SignupHandle(json &requestInfo)
{
    ORMapper<UserInfo> mapper(DATABASE_NAME);
    UserInfo helper;
    QueryMessager<UserInfo> messager(helper);
    json responseInfo;

    std::cout << "[INFO] Signup request comes" << std::endl;

    auto result = mapper.Query(messager
                 .Where(Field(helper.username) == requestInfo["username"].get<std::string>()));
    //std::cout << "[INFO] Query finish" << std::endl;

    if (result)
    {
        if (messager.IsNone())
        {
            // 构建新的用户信息
            UserInfo userinfo = { requestInfo["username"].get<std::string>(),
                        requestInfo["password"].get<std::string>(), 0, 0, 3, 0};
            auto result = mapper.Insert(userinfo);
            if (result)
            {
                // 随机分发三个小精灵
                auto InitialBag = [&]() {
                    for (int i = 0; i < 3; i++)
                    {
                        DispatchPokemon(requestInfo["username"].get<std::string>());
                    }
                };


                responseInfo["define"] = SIGN_UP_SUCCESS;
                InitialBag();
            }
            else
                responseInfo["define"] = SIGN_UP_FAIL;
        }
        else
            responseInfo["define"] = SIGN_UP_FAIL;
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::OnlineListHandle(json &requestInfo)
{
    json responseInfo;

    try{
        responseInfo["info"] = json(_parent->GetOnlineList());
        responseInfo["define"] = QUERY_SUCCESS;
    }
    catch (std::exception e)
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << e.what() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::OfflineListHandle(json &requestInfo)
{
    json responseInfo;

    ORMapper<UserInfo> mapper(DATABASE_NAME);
    UserInfo helper;
    QueryMessager<UserInfo> messager(helper);

    std::list<std::string> userList;
    std::list<std::string> offlineList;

    auto result = mapper.Query(messager);
    if (result)
    {
        for (const auto& vec: messager.GetVector())
            userList.emplace_back(vec[0]);

        std::list<std::string> onlineList(_parent->GetOnlineList());
        userList.sort();
        onlineList.sort();
        std::set_difference(userList.begin(), userList.end(),
                         onlineList.begin(), onlineList.end(),
                              std::back_inserter(offlineList));

        responseInfo["define"] = QUERY_SUCCESS;
        responseInfo["info"] = json(std::move(offlineList));
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::UserBagHandle(json &requestInfo)
{
    ORMapper<UserBag> mapper(DATABASE_NAME);
    UserBag helper;
    QueryMessager<UserBag> messager(helper);

    // 查询用户背包信息
    auto result = mapper.Query(messager
                               .Where(Field(helper.username)
                                      == requestInfo["username"].get<std::string>()));
    json responseInfo;
    if (result)
    {
        responseInfo["define"] = QUERY_SUCCESS;
        json itemsInfo;
        for (const auto& vec: messager.GetVector())
        {
            json itemInfo = {
                {"id", vec[0]},
                {"name", vec[1]},
                {"level", vec[2]},
                {"exp", vec[3]},
                {"type", vec[4]},
                {"attackPoint", vec[5]},
                {"defensePoint", vec[6]},
                {"healthPoint", vec[7]},
                {"attackFrequence", vec[8]},
                {"property", vec[9]},
            };
            itemsInfo.push_back(itemInfo.dump());
        }
        responseInfo["info"] = itemsInfo;

        ORMapper<UserInfo> mapper(DATABASE_NAME);
        UserInfo helper;
        QueryMessager<UserInfo> messager(helper);

        // 查询用户胜率
        auto result = mapper.Query(messager
                                   .Where(Field(helper.username)
                                          == requestInfo["username"].get<std::string>()));
        if (result)
        {
            auto win = std::stoi(messager.GetVector()[0][2]);
            auto lose = std::stoi(messager.GetVector()[0][3]);

            responseInfo["rate"] = (win + lose == 0) ? 100 : (win * 100.0) / (win + lose);
        }
        else
        {
            responseInfo["define"] = SERVER_ERROR;
            std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
        }
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::UserAchievementHandle(json &requestInfo)
{
    ORMapper<UserInfo> mapper(DATABASE_NAME);
    UserInfo helper;
    QueryMessager<UserInfo> messager(helper);

    // 查询用户背包信息
    auto result = mapper.Query(messager
                               .Where(Field(helper.username)
                                      == requestInfo["username"].get<std::string>()));
    json responseInfo;
    if (result)
    {
        auto win = std::stoi(messager.GetVector()[0][2]);
        auto lose = std::stoi(messager.GetVector()[0][3]);

        responseInfo["define"] = QUERY_SUCCESS;
        responseInfo["rate"] = (win + lose == 0) ? 100 : (win * 100.0) / (win + lose);
        responseInfo["sum_ach"] = std::stoi(messager.GetVector()[0][4]);
        responseInfo["advance_ach"] = std::stoi(messager.GetVector()[0][5]);
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::PokemonListHandle(json &requestInfo)
{
    ORMapper<PokemonList> mapper(DATABASE_NAME);
    PokemonList helper;
    QueryMessager<PokemonList> messager(helper);

    // 查询对战精灵名字
    auto result = mapper.Query(messager);
    json responseInfo;
    if (result)
    {
        responseInfo["define"] = QUERY_SUCCESS;
        std::list<std::string> pokemonList;
        for (const auto& vec: messager.GetVector())
        {
            pokemonList.push_back(vec[0]);
        }
        responseInfo["info"] = json(pokemonList);
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::PokemonInfoHandle(json &requestInfo)
{
    ORMapper<PokemonList> mapper(DATABASE_NAME);
    PokemonList helper;
    QueryMessager<PokemonList> messager(helper);

    // 查询对战小精灵信息
    auto result = mapper.Query(messager
                               .Where(Field(helper.name)
                                      == requestInfo["name"].get<std::string>()));
    json responseInfo;
    if (result)
    {
        auto vec = messager.GetVector()[0];
        responseInfo["define"] = QUERY_SUCCESS;
        json itemInfo = {
            {"name", vec[0]},
            {"type", vec[1]},
            {"attackPoint", vec[2]},
            {"defensePoint", vec[3]},
            {"healthPoint", vec[4]},
            {"attackFrequence", vec[5]},
            {"property", vec[6]},
            {"exp", vec[7]}
        };
        responseInfo["info"] = itemInfo.dump();
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::GameWinHandle(json &requestInfo)
{
    json responseInfo;

    ORMapper<PokemonInfo> infoMapper(DATABASE_NAME);
    PokemonInfo helper;
    QueryMessager<PokemonInfo> infoMessager(helper);

    infoMapper.Query(infoMessager.Where(Field(helper.name)
                                        == requestInfo["get"].get<std::string>()));
    if (infoMessager.IsNone())
        throw std::runtime_error("Unknown name for pokemon");
    else
    {
        auto vec = infoMessager.GetVector()[0];
        ORMapper<UserBag> bagMapper(DATABASE_NAME);
        // Bad code style, wait for ormlite to support for GetObjects()
        UserBag userBag = {nullptr , vec[1],
                            1, 0, std::stoi(vec[2]), std::stoi(vec[3]), std::stoi(vec[4]),
                            std::stoi(vec[5]), std::stoi(vec[6]), vec[7], _username};

        ORMapper<UserInfo> userMapper(DATABASE_NAME);
        UserInfo helper;
        QueryMessager<UserInfo> userMessager(helper);
        // 获得用户信息
        userMapper.Query(userMessager.Where(Field(helper.username) == _username));
        vec = userMessager.GetVector()[0];
        UserInfo userInfo = {vec[0] , vec[1], std::stoi(vec[2]), std::stoi(vec[3]),
                             std::stoi(vec[4]), std::stoi(vec[5])};

        // 放入新获得的小精灵
        if (!bagMapper.Insert(userBag))
        {
            responseInfo["define"] = SERVER_ERROR;
            throw std::runtime_error("Failed at insert userbag: " + userMapper.GetErrorMessage());
        }
        else
        {
            responseInfo["define"] = ACCEPT;

            // 更新用户信息
            userInfo.win += 1;
            userInfo.sum += 1;
        }
        if (requestInfo["level"].get<int>() == 15)
            userInfo.advanceSum += 1;
        if (!userMapper.Update(userInfo))
        {
            responseInfo["define"] = SERVER_ERROR;
            throw std::runtime_error("Failed at update user info");
        }

        // 更新战斗精灵的信息
        // Bad code style, wait for ormlite to support for GetObjects()
        UserBag updateItem = {
            requestInfo["id"].get<int>(),
            requestInfo["name"].get<std::string>(),
            requestInfo["level"].get<int>(),
            requestInfo["exp"].get<int>(),
            requestInfo["type"].get<int>(),
            requestInfo["attackPoint"].get<int>(),
            requestInfo["defensePoint"].get<int>(),
            requestInfo["healthPoint"].get<int>(),
            requestInfo["attackFrequence"].get<int>(),
            requestInfo["property"].get<std::string>(),
            _username
        };
        if (!bagMapper.Update(updateItem))
        {
            responseInfo["define"] = SERVER_ERROR;
            throw std::runtime_error("Failed at update pokemon info");
        }

    }

    return std::move(responseInfo);
}

json Dispatcher::GameLoseHandle(json &requestInfo)
{
    json responseInfo;

    ORMapper<UserInfo> userMapper(DATABASE_NAME);
    UserInfo helper;
    QueryMessager<UserInfo> userMessager(helper);

    userMapper.Query(userMessager.Where(Field(helper.username) == _username));
    auto vec = userMessager.GetVector()[0];
    UserInfo userInfo = {vec[0] , vec[1], std::stoi(vec[2]), std::stoi(vec[3]),
                         std::stoi(vec[4]), std::stoi(vec[5])};
    userInfo.lose += 1;
    if (userMapper.Update(userInfo))
        responseInfo["define"] = ACCEPT;
    else
        responseInfo["define"] = SERVER_ERROR;

    if (requestInfo["isLose"].get<bool>() == false)
        return std::move(responseInfo);
    else
    {
        ORMapper<UserBag> infoMapper(DATABASE_NAME);
        UserBag helper;
        QueryMessager<UserBag> infoMessager(helper);

        infoMapper.Query(infoMessager.Where(
                             Field(helper.username) == _username));

        if (infoMessager.IsNone())
            throw std::runtime_error("Unknown username");
        else
        {
            auto vec = infoMessager.GetVector();
            std::unordered_set<int> set;
            json itemsInfo;

            auto random = [&](int num) {
                auto ret = rand() % (num + 1);

                while (set.insert(ret).second != true)
                    ret = rand() % (num + 1);
                return ret;
            };

            for (int i = 0; i < 3 && i < vec.size(); i++)
            {
                auto pos = random(vec.size() - 1);
                auto item = vec.at(pos);

                json itemInfo = {
                    {"id", item[0]},
                    {"name", item[1]},
                    {"level", item[2]},
                    {"exp", item[3]},
                    {"type", item[4]},
                    {"attackPoint", item[5]},
                    {"defensePoint", item[6]},
                    {"healthPoint", item[7]},
                    {"attackFrequence", item[8]},
                    {"property", item[9]},
                };
                itemsInfo.push_back(itemInfo.dump());
            }
            responseInfo["info"] = itemsInfo;
        }
        return std::move(responseInfo);
    }


}

json Dispatcher::LosePokemonHandle(json &requestInfo)
{
    json responseInfo;

    ORMapper<UserBag> bagMapper(DATABASE_NAME);
    UserBag helper;
    QueryMessager<UserBag> bagMessager(helper);

    // 获得小精灵的等级
    bagMapper.Query(bagMessager.Where(Field(helper.id) == requestInfo["id"].get<int>()));
    auto level = bagMessager.GetVector()[0][2];

    // 删除指定的小精灵
    if (bagMapper.Delete("id", std::to_string(requestInfo["id"].get<int>())))
    {
        responseInfo["define"] = ACCEPT;

        bagMapper.Query(bagMessager.Where(
                             Field(helper.username) == _username));

        ORMapper<UserInfo> userMapper(DATABASE_NAME);
        UserInfo helper;
        QueryMessager<UserInfo> userMessager(helper);

        userMapper.Query(userMessager.Where(Field(helper.username) == _username));
        auto vec = userMessager.GetVector()[0];
        UserInfo userInfo = {vec[0] , vec[1], std::stoi(vec[2]), std::stoi(vec[3]),
                             std::stoi(vec[4]), std::stoi(vec[5])};

        if (std::stoi(level) == 15)
        {
            userInfo.advanceSum += 1;
        }
        // 用户如果已经没有小精灵，则再随机分配一个初始小精灵
        if (bagMessager.IsNone())
        {
            DispatchPokemon(_username);
        }
        else
            userInfo.sum -= 1;

        userMapper.Update(userInfo);
    }
    else
        responseInfo["define"] = SERVER_ERROR;

    return responseInfo;
}

void Dispatcher::DispatchPokemon(std::string username)
{
    auto random = [](int num) {
        return rand() % (num + 1);
    };


    ORMapper<PokemonInfo> infoMapper(DATABASE_NAME);
    PokemonInfo helper;
    QueryMessager<PokemonInfo> infoMessager(helper);


    if (!infoMapper.Query(infoMessager.Where(Field(helper.id) == random(N))))
    {
        throw std::runtime_error("Failed at query pokemoninfo: "
                                 + infoMapper.GetErrorMessage());
    }

    if (infoMessager.IsNone())
        throw std::runtime_error("Unknown id for pokemon");
    else
    {
        auto vec = infoMessager.GetVector()[0];
        ORMapper<UserBag> userMapper(DATABASE_NAME);
        // Bad code style, wait for ormlite to support for GetObjects()
        UserBag userBag = { nullptr, vec[1], 1, 0, std::stoi(vec[2]),
                            std::stoi(vec[3]), std::stoi(vec[4]),
                            std::stoi(vec[5]), std::stoi(vec[6]),
                            vec[7], username};
        if (!userMapper.Insert(userBag))
        {
            throw std::runtime_error("Failed at insert userbag: " + userMapper.GetErrorMessage());
        }
    }
}

void Dispatcher::Logout()
{
    _parent->Offline(_username);
}
