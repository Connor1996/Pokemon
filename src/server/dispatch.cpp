#include "dispatch.h"
#include "ormlite.h"
#include "../define.h"
#include "../model/userinfo.h"

using namespace ORMLite;

std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;

    if (requestInfo["type"].get<int>() == LOG_IN)
    {
        ORMapper<UserInfo> mapper("data.db");
        QueryMessager<UserInfo> messager;

        auto result = mapper.Query(messager
                     .Where(Field(UserInfo{}.username) == requestInfo["username"].get<std::string>()
                            && Field(UserInfo{}.password) == requestInfo["password"].get<std::string>()));

        std::cout << "[INFO] Login request comes" << std::endl;
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

    }
    else
    {
        responseInfo["type"] = SERVER_ERROR;
        std::cout << "[ERROR] Bad request" << std::endl;
    }

    return std::move(responseInfo.dump());
}
