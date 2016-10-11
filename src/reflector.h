#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <map>

class Pokemon;
typedef struct struct_Attribute Attribute;
typedef Pokemon* (*PTRCreateObject)(std::string, unsigned int, unsigned int, Attribute);

#define REGISTER(_CLASS_NAME_)                                          \
_CLASS_NAME_* Create##_CLASS_NAME_(std::string name, unsigned int level,\
    unsigned int exp, Attribute attribute)                              \
{                                                                       \
    return new _CLASS_NAME_(name, level, exp, attribute);               \
}                                                                       \
                                                                        \
RegisterAction createRegister##_CLASS_NAME_(                            \
    #_CLASS_NAME_, (PTRCreateObject)Create##_CLASS_NAME_)

class Reflector
{
public:
    static Reflector& GetInstance();
    PTRCreateObject GetClassByName(std::string className);

    void RegistClass(std::string className, PTRCreateObject method);
private:
    std::map<std::string, PTRCreateObject> _classMap;
    Reflector() {}
};

class RegisterAction
{
public:
    RegisterAction(std::string className,PTRCreateObject PtrCreateFn);
};


#endif // REFLECTOR_H
