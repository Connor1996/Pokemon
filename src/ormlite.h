#ifndef ORMLITE_H
#define ORMLITE_H

#include "include/sqlite3.h"

#include <string>
#include <vector>
#include <exception>

// 序列化所用的分隔符
#define SEPARATOR ','

#define ORMAP(_CLASS_NAME_, ...)                                \
private:                                                        \
friend class ORMLite::ORMapper<_CLASS_NAME_>;                   \
template <typename VISITOR>                                     \
void _Accept(const VISTIOR &visitor)                            \
{                                                               \
    visitor.visit(__VA_ARGS__);                                 \
}                                                               \
constexpr static const std::string _CLASSNAME = #_CLASS_NAME_;  \
constexpr static const std::string _FIELDSNAME = #__VA_ARGS__


namespace ORMLite {

template <typename T>
class ORMapper
{
public:
    ORMapper(const std::string &dbName)
        : _dbName(dbName), _tableName(T::_CLASSNAME),
          _fieldsName(ORMLite_Impl::_Split(T::_FILEDSNAME))
    { }

    bool CreateTable()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector &connector)
        {
            // 创建model类，使用访问者访问获得类中元信息
            const T modelObject {};
            ORMLite_Impl::TypeVisitor vistor;
            modelObject._Accept(vistor);

            auto fieldsType = ORMLite_Impl::_Split(vistor.GetSerializedStr());
            std::vector::size_type indexOfFields = 0;

            auto FieldsStr = _fieldsNames[0] + " " +
                    fieldsType[0] + " PRIMARY KEY NOT NULL,";
            while (indexOfFields++ < _fieldsName.size())
            {
                FieldsStr += _fieldsName[indexOfFields] + " " +
                        _fieldsType[indexOfFields] + ",";
            }
            auto commandStr = "CREATE TABLE " + _tableName +
                    " IF NOT EXISTS" + "(" + FieldsStr + ");";

            connector.Excute(commandStr);
        })
    }

    bool DropTable()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector &connector)
        {
            connector.Excute("DROP TABLE " + _tableName + ";");
        })
    }

    bool Select(const std::string &property)
    {
        return _HandleException([&] (ORMlite_Impl::SQLConnector &connector)
        {

        })
    }

    bool Insert(const )
    // 读入指定的对象，根据该对象的主键删除整行
    bool Delete(const T &modelObject)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector &connector)
        {
            ORMLite_Impl::ReaderVisitor visitor;
            modelObject._Accept(visitor);

            // 格式如 "property = value"
            auto KeyValueStr = _fieldNames[0] + "=" + visitor.GetSerializedStr();
            connector.Excute("DELETE FROM " + _tableName + " " +
                             "WHERE " + formatStr + ";");
        })
    }

    bool Update(const T &modelObject)
    {
        return _HandleException([&] (ORMLite::Impl::SQLConnector &connector)
        {

        })
    }

    // 删除所有行
    bool DeleteAll()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector &connector)
        {
            connector.Excute("DELETE FROM " + _tableName + ";");
        })
    }

    bool Update()
    {

    }

private:
    const std::string _dbName;
    const std::string _tableName;
    const std::vector<std::string> _fieldNames;
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

    static std::vector<std::string> _Split(std::string str)
    {
        std::vector<std::string> result;
        std::string tempStr = "";
        // 扩展字符串方便操作
        str += ',';

        for (const auto &ch : str)
        {
            switch (ch)
            {
            case SEPARATOR:
            case ',':
                result.push_back(tempStr);
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

class TypeVisitor
{
public:
    template <typename ...Args>
    inline void Visit(Args & ...args)
    {
        return _Visit(args...);
    }

    std::string GetSerializedStr() const { return _serialized; }
private:
    std::string _serializedStr;

    template <typename First, typename ...Args>
    inline void _Visit(First& property, Args & ...args)
    {
        _Visit(property);
        _Visit(args);
    }

    inline void _Visit(const int &property)
    {
        _serializedStr += "INT";
        _serializedStr += SEPARATOR;
    }
    inline void _Visit(const double &property)
    {
        _serializedStr += "REAL";
        _serializedStr += SEPARATOR;
    }
    inline void _Visit(const std::string &property)
    {
        _serializedStr += "TEXT";
        _serializedStr += SEPARATOR;
    }
};

class ReaderVisitor
{
public:
    template <typename ...Args>
    inline void Visit(Args & ...args)
    {
        return _Visit(args...);
    }

    std::string GetSerializedStr() const { return _serialized; }
private:
    std::tuple _serializedStr;

    template <typename First, typename ...Args>
    inline void _Visit(First &property, Args &...args)
    {
        _Visit(property);
        _Visit(args);
    }

    inline void _Visit(const int &property)
    {
        _serializedStr = std::to_string(property);
    }
    inline void _Visit(const double &property)
    {
        _serializedStr += std::to_string(property);
    }
    inline void _Visit(const std::string &property)
    {
        _serializedStr += "'" + property + "'";
    }

    _
};
}
#endif // ORMLITE_H
