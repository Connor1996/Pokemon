#include "sqlconnector.h"
#include <stdexcept>

// 类的静态成员外部初始化
SQLConnector::Garbo SQLConnector::garbo;

SQLConnector::SQLConnector()
{
    if (sqlite3_open(FILE_NAME, &_database))
        throw std::runtime_error(
                std::string("Can't open database: %s\n")
                + sqlite3_errmsg(_database));
}

// 类的静态成员需要在.c中定义初始化，.h中只是声明
SQLConnector *SQLConnector::_instance = NULL;

SQLConnector *SQLConnector::GetInstance()
{
    if (_instance == NULL)
        _instance = new SQLConnector();
    return _instance;
}
