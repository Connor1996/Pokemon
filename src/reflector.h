#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <map>

class Pokemon;
typedef struct struct_Attribute Attribute;

// 可以生成Pokemon对象的函数指针
typedef Pokemon* (*PTRCreateObject)(std::string, unsigned int, unsigned int, Attribute, int);

// 新建全局函数以new类对象，模拟反射
#define REGISTER(_CLASS_NAME_)                                          \
_CLASS_NAME_* Create##_CLASS_NAME_(std::string name, unsigned int level,\
    unsigned int exp, Attribute attribute, int id)                      \
{                                                                       \
    return new _CLASS_NAME_(name, level, exp, attribute, id);           \
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
