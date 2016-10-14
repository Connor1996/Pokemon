#ifndef ORMLITE_H
#define ORMLITE_H

#include "include/sqlite3.h"

#include <string>
#include <set>
#include <exception>

#define ORMAP(_CLASS_NAME_, ...)                                \
template <typename VISITOR>                                     \
private:                                                        \
friend class ORMLite::ORMapper<_CLASS_NAME_>;                   \
void Accept(VISTIOR &visitor)                                   \
{                                                               \
    visitor.visit(__VA_ARGS__);                                 \
}                                                               \
constexpr static const std::string _CLASSNAME = #_CLASS_NAME_;  \
constexpr static const std::string _FIELDNAMES = #__VA_ARGS__


namespace ORMLite {

template <typename T>
class ORMapper
{
public:
    ORMapper(const std::string &dbName)
        : _dbName(dbName), _tableName(T::_CLASSNAME),
          _fieldNames(_split(T::_FILEDNAMES))
    { }

    bool CreateTable()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector &connector)
        {

        })
    }

    bool DropTable()
    {

    }

    bool Select()
    {

    }

    bool Delete()
    {

    }

    bool Update()
    {

    }

private:
    const std::string _dbName;
    const std::string _tableName;
    const std::set<std::string> _fieldNames;
    std::string _errorMessage;

    bool _HandleException(std::function<void (ORMLite_Impl::SQLConnector &)> fn)
    {
        try
        {
            ORMLite_Impl::SQLConnector connector(_dbName);
            fn(connector);
            return true;
        }
        catch (const std::exception &e)
        {
            _errorMessage = e.what();
            return false;
        }
    }

    static std::set<std::string> _Split(std::string str)
    {
        std::set<std::string> result;
        std::string tempStr = "";
        // 扩展字符串方便操作
        str += ',';

        for (const auto &ch : str)
        {
            switch (ch)
            {
            case ',':
                result.insert(tempStr);
                tempStr.clear();
                break;
            case ' ':
            case '_':
                break;
            default:
                tempStr += ch;
                break;
            }
        }
        return std::move(result);
    }
};

}

namespace ORMLite_Impl {

class SQLConnector
{
public:
    SQLConnector(const std::string &fileName)
    {
        if (sqlite3_open(fileName, &_database))
        {
            sqlite3_close(_databse);
            throw std::runtime_error(
                        std::string("Can't open database: %s\n",
                                    sqlite3_errmsg(_database)));
        }
    }

    ~SQLConnector()
    {
        sqlite3_close(_database);
    }

private:
    sqlite3* _database;
};
}
#endif // ORMLITE_H
