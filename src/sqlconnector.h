#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H

#include "include/sqlite3.h"

#define FILE_NAME "f"

class SQLConnector
{
public:
    static SQLConnector *GetInstance();

private:
    SQLConnector();
    static SQLConnector *_instance;
    sqlite3 *_database;

    // 垃圾回收类，在析构函数中删除SQLConnector的单例
    class Garbo
    {
    public:
        ~Garbo()
        {
            if(SQLConnector::_instance)
                delete SQLConnector::_instance;
        }
    };
    // 定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数
    static Garbo garbo;
};

#endif // SQLCONNECTOR_H
