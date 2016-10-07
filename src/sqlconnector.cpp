#include "sqlconnector.h"

SQLConnector::SQLConnector()
{
    if (sqlite3_open(FILE_NAME, &_database))
        throw std::runtime_error(
                std::string("Can't open database: %s\n")
                + sqlite3_errmsg(_database));
}

SQLConnector *SQLConnector::_instance = NULL;
SQLConnector *SQLConnector::GetInstance()
{
    if (_instance == NULL)
        _instance = new SQLConnector();
    return _instance;
}
