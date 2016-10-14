#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H

#include "include/sqlite3.h"
#include <tuple>

#define FILE_NAME "pokemon.db"

class SQLConnector
{
public:
    static SQLConnector& GetInstance();
    ~SQLConnector();
   // std::tuple<std::string, Attribute> QueryInformation(std::string name);
private:
    SQLConnector();
    // 禁用复制构造函数
    SQLConnector(const SQLConnector &) = delete;
    // 禁用赋值操作符
    SQLConnector& operator=(const SQLConnector &) = delete;

    sqlite3 *_database;


};

#endif // SQLCONNECTOR_H
