#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <map>

class Pokemon;
typedef struct struct_Attribute Attribute;

// 定义生成Pokemon对象的函数指针
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


// 反射类，维护类名与之对应构造函数的map
class Reflector
{
public:
    // 使用单例模式，获得Reflector实例
    // @return:
    //      Reflector唯一对象的引用
    static Reflector& GetInstance()
    {
        static Reflector instance;
        return instance;
    }

    // 通过类名获得类的构造函数
    // @param:
    //      className 需要获得的类名
    // @return:
    //      生成相应类的函数指针
    PTRCreateObject GetClassByName(std::string className)
    {
        auto iter = _classMap.find(className);
        if (iter == _classMap.end())
            return NULL;
        else
            return iter->second;
    }

    // 将类名和构造函数注册到map中
    // @param:
    //      className 需要注册的类名
    //      method 生成相应类的函数指针
    void RegistClass(std::string className, PTRCreateObject method)
    {
        _classMap.insert(std::pair<std::string, PTRCreateObject>(className, method));
    }
private:
    // 类名与之对应构造函数的map
    std::map<std::string, PTRCreateObject> _classMap;

    // 隐藏Reflector的构造函数，单例模式只运行使用GetInstance获得Reflector唯一实例
    Reflector() {}
};

// 注册行为类
// 通过构造函数以实现自动向Reflector注册
class RegisterAction
{
public:
    RegisterAction(std::string className,PTRCreateObject PtrCreateFn)
    {
        Reflector::GetInstance().RegistClass(className, PtrCreateFn);
    }

};


#endif // REFLECTOR_H
