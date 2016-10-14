#include "sqlconnector.h"
#include <stdexcept>

// 类的静态成员外部初始化
SQLConnector::SQLConnector()
{
    if (sqlite3_open(FILE_NAME, &_database) != SQLITE_OK)
        throw std::runtime_error(
                std::string("Can't open database: %s\n")
                + sqlite3_errmsg(_database));
}

SQLConnector& SQLConnector::GetInstance()
{
    static SQLConnector _instance;
    return _instance;
}

SQLConnector::~SQLConnector()
{
    sqlite3_close(_database);
}

