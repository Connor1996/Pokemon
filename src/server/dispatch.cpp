#include "dispatch.h"
#include "ormlite.h"
#include "../define.h"
#include "../model/userinfo.h"

using namespace ORMLite;

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
            responseInfo["type"] = LOG_IN_FAIL;
        else
            responseInfo["type"] = LOG_IN_SUCCESS;
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
    ORMapper<UserInfo> mapper("data.db");
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
                responseInfo["type"] = SIGN_UP_SUCCESS;
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
