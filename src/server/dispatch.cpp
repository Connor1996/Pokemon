#include "dispatch.h"
#include "ormlite.h"

#include "../define.h"
#include "../model/userinfo.h"
#include "../model/userbag.h"
#include "../model/pokemoninfo.h"

#include <stdexcept>
#include <algorithm>
#include <set>

using namespace ORMLite;

// 数据库中小精灵的种类数
#define N 4

std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;
    //std::cout << "[INFO] dispatch" << std::endl;
    switch (requestInfo["type"].get<int>()) {
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
    default:
        responseInfo["type"] = SERVER_ERROR;
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
            responseInfo["type"] = LOG_IN_FAIL_WP;
        else
        {
            // 将username加入在线列表
            _username = requestInfo["username"].get<std::string>();
            if (_parent->Online(_username, _connection))
                responseInfo["type"] = LOG_IN_SUCCESS;
            else
                responseInfo["type"] = LOG_IN_FAIL_AO;
        }
    }
    else
    {
        responseInfo["type"] = SERVER_ERROR;
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
            UserInfo userinfo = { requestInfo["username"].get<std::string>(),
                        requestInfo["password"].get<std::string>() };
            auto result = mapper.Insert(userinfo);
            if (result)
            {
                //产生0-num中的整数
                auto random = [](int num) {
                    srand(static_cast<int>(time(NULL)));
                    return rand() % (num + 1);
                };

                // 随机分发三个小精灵
                auto InitialBag = [&]() {
                    ORMapper<PokemonInfo> infoMapper(DATABASE_NAME);
                    PokemonInfo helper;
                    QueryMessager<PokemonInfo> infoMessager(helper);


                    for (int i = 0; i < 3; i++)
                    {
                        infoMessager.Clear();

                        infoMapper.Query(infoMessager.Where(Field(helper.id) == random(N)));
                        if (infoMessager.IsNone())
                            throw std::runtime_error("Unknown id for pokemon");
                        else
                        {
                            auto vec = infoMessager.GetVector()[0];
                            ORMapper<UserBag> userMapper(DATABASE_NAME);
                            // Bad code style, wait for ormlite to support for GetObjects()
                            UserBag userBag = { requestInfo["username"].get<std::string>(), vec[1],
                                                1, 0, std::stoi(vec[2]), std::stoi(vec[3]), std::stoi(vec[4]),
                                                std::stoi(vec[5]), std::stoi(vec[6]) };
                            if (!userMapper.Insert(userBag))
                            {
                                throw std::runtime_error("Failed at insert userbag: " + userMapper.GetErrorMessage());
                            }
                        }
                    }
                };

                responseInfo["type"] = SIGN_UP_SUCCESS;
                InitialBag();
            }
            else
                responseInfo["type"] = SIGN_UP_FAIL;
        }
        else
            responseInfo["type"] = SIGN_UP_FAIL;
    }
    else
    {
        responseInfo["type"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::OnlineListHandle(json &requestInfo)
{
    json responseInfo;

    try{
        responseInfo["info"] = json(_parent->GetOnlineList());
        responseInfo["type"] = QUERY_SUCCESS;
    }
    catch (std::exception e)
    {
        responseInfo["type"] = SERVER_ERROR;
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
        std::set_difference(userList.begin(), userList.end(),
                         onlineList.begin(), onlineList.end(),
                              std::back_inserter(offlineList));

        responseInfo["type"] = QUERY_SUCCESS;
        responseInfo["info"] = json(std::move(offlineList));
    }
    else
    {
        responseInfo["type"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }

    return std::move(responseInfo);
}

json Dispatcher::UserBagHandle(json &requestInfo)
{
    ORMapper<UserBag> mapper(DATABASE_NAME);
    UserBag helper;
    QueryMessager<UserBag> messager(helper);


    auto result = mapper.Query(messager
                               .Where(Field(helper.username)
                                      == requestInfo["username"].get<std::string>()));
    json responseInfo;
    if (result)
    {
        responseInfo["type"] = QUERY_SUCCESS;
        json itemsInfo;
        for (const auto& vec: messager.GetVector())
        {
            json itemInfo = {
                {"name", vec[1]},
                {"level", vec[2]},
                {"type", vec[4]},
                {"attackPoint", vec[5]},
                {"defensePoint", vec[6]},
                {"healthPoint", vec[7]},
                {"attackFrequence", vec[8]}
            };
            itemsInfo.push_back(itemInfo.dump());
        }
        responseInfo["info"] = itemsInfo;
    }
    else
    {
        responseInfo["type"] = SERVER_ERROR;
        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
    }



    return std::move(responseInfo);
}

void Dispatcher::Logout()
{
    _parent->Offline(_username);
}
