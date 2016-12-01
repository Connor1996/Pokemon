#ifndef DISPATCH_H
#define DISPATCH_H

#include "include/json.hpp"

using json = nlohmann::json;

class Dispatcher
{
public:
    Dispatcher() : _state(0) { }

    // 根据请求信息，分发到相应的函数处理请求
    std::string Dispatch(json requestInfo);

    json LoginHandle(json&);
    json SignupHandle(json&);
private:
    int _state;
};

#endif // DISPATCH_H
