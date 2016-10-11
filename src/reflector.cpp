#include "reflector.h"

Reflector& Reflector::GetInstance()
{
    static Reflector instance;
    return instance;
}

PTRCreateObject Reflector::GetClassByName(std::string className)
{
    auto iter = _classMap.find(className);
    if (iter == _classMap.end())
        return NULL;
    else
        return iter->second;
}

void Reflector::RegistClass(std::string className, PTRCreateObject method)
{
    _classMap.insert(std::pair<std::string, PTRCreateObject>(className, method));
}

RegisterAction::RegisterAction(std::string className,PTRCreateObject PtrCreateFn)
{
    Reflector::GetInstance().RegistClass(className, PtrCreateFn);
}
