#ifndef ORMLITE_H
#define ORMLITE_H

#include "include/sqlite3.h"

#include <string>
#include <vector>
#include <exception>
#include <thread>

// 序列化所用的分隔符
#define SEPARATOR ','

#define ORMAP(_CLASS_NAME_, ...)                                \
private:                                                        \
friend class ORMLite::ORMapper<_CLASS_NAME_>;                   \
template <typename VISITOR>                                     \
void _Accept(const VISITOR& visitor)                            \
{                                                               \
    visitor.visit(__VA_ARGS__);                                 \
}                                                               \
constexpr static const char* _CLASSNAME = #_CLASS_NAME_;  \
constexpr static const char* _FIELDSNAME = #__VA_ARGS__

namespace ORMLite_Impl {

class SQLConnector
{
public:
    SQLConnector(const std::string& fileName)
    {
        if (sqlite3_open(fileName.c_str(), &_database))
        {
            sqlite3_close(_database);
            throw std::runtime_error(
                        std::string("Can't open database: %s\n",
                                    sqlite3_errmsg(_database)));
        }
    }

    ~SQLConnector()
    {
        sqlite3_close(_database);
    }

    void Execute(const std::string &sqlStr,
                 std::function<void (int, char**, char**)> callback = _callback)
    {
        const size_t MAX_TRIAL = 6;
        char *errMsg = 0;
        int result;

        auto callbackWrapper = [] (void* fn, int column,
                char** columnText, char** columnName) -> int
        {
            static_cast<std::function<void (int, char**, char**)> *>
                    (fn)->operator()(column, columnText, columnName);
            return 0;
        };

        // 若数据库繁忙，等待后重试
        for (size_t i = 0; i < MAX_TRIAL; i++)
        {
            result = sqlite3_exec(_database, sqlStr.c_str(), callbackWrapper, &callback, &errMsg);
            if (result != SQLITE_BUSY)
                break;

            std::this_thread::sleep_for(std::chrono::microseconds(20));
        }

        if (result != SQLITE_OK)
        {
            auto errStr = "SQL Error: " + std::string(errMsg);

            // errMsg由sqlite3_malloc()产生，所以需要手动sqlite3_free()
            sqlite3_free(errMsg);
            throw std::runtime_error(errStr);
        }


    }

private:
    sqlite3* _database;

    static void _callback(int column, char** columnText, char** columnName)
    {
        return;
    }
};

struct Expr
{
    std::vector<std::pair<const void *, std::string>> expr;

    Expr (const std::string& property, const std::string& op,
         const std::string& value)
        : expr{ std::make_pair(&property, op + "'" + value + "'") }
    { }

    template <typename T>
    Expr (const T& property, const std::string& op, const T& value)
        : expr{ std::make_pair(&property, op + std::to_string(value)) }
    { }

    inline Expr operator &&(const Expr& rhs)
    {
        return _OperatorToString(rhs, " AND ");
    }

    inline Expr operator ||(const Expr& rhs)
    {
        return _OperatorToString(rhs, " OR ");
    }

private:
    Expr _OperatorToString(const Expr& rhs, std::string op)
    {
        // 前面加括号，最后面也要加括号，以确保运算顺序
        expr.emplace(expr.begin(), std::make_pair(nullptr, "("));
        expr.emplace_back(std::make_pair(nullptr, std::move(op)));
        for (const auto exprPair : rhs.expr)
            expr.emplace_back(exprPair);
        expr.emplace_back(std::make_pair(nullptr, ")"));

        return *this;
    }
};

// 每个属性都由Field()修饰，即创建Field_Expr对象
// 重载Field_Expr的操作符，使得布尔表达式转换成自定义的Expr对象
// 以满足生成相应的sql语句
template <typename T>
struct Field_Expr
{
    const T& _property;

    Field_Expr(const T& property)
        : _property(property)
    {}

    inline Expr operator ==(T value)
    {
        return Expr{_property, "=", std::move(value)};
    }

    inline Expr operator !=(T value)
    {
        return Expr{_property, "!=", std::move(value)};
    }

    inline Expr operator >(T value)
    {
        return Expr{_property, ">", std::move(value)};
    }

    inline Expr operator <(T value)
    {
        return Expr{_property, "<", std::move(value)};
    }

    inline Expr operator >=(T value)
    {
        return Expr{_property, ">=", std::move(value)};
    }

    inline Expr operator <=(T value)
    {
        return Expr{_property, "<=", std::move(value)};
    }

};

class TypeVisitor
{
public:
    template <typename ...Args>
    inline void Visit(Args&  ...args)
    {
        return _Visit(args...);
    }

    std::string GetSerializedStr() const { return _serializedStr; }

protected:
    std::string _serializedStr;

    template <typename First, typename ...Args>
    inline void _Visit(First& property, Args&  ...args)
    {
        _Visit(property);
        _Visit(args...);
    }

    inline void _Visit(const int& property)
    {
        _serializedStr += "INT" + SEPARATOR;
    }
    inline void _Visit(const double& property)
    {
        _serializedStr += "REAL" + SEPARATOR;
    }
    inline void _Visit(const std::string& property)
    {
        _serializedStr += "TEXT" + SEPARATOR;
    }
};

class ReaderVisitor
{
public:
    template <typename ...Args>
    inline void Visit(Args&  ...args)
    {
        return _Visit(args...);
    }

    std::string GetSerializedStr() const { return _serializedStr; }

protected:
    std::string _serializedStr;

    template <typename First, typename ...Args>
    inline void _Visit(First& property, Args& ...args)
    {
        _Visit(property);
        _Visit(args...);
    }


    inline void _Visit(const int& property)
    {
        _serializedStr += std::to_string(property) + SEPARATOR;
    }
    inline void _Visit(const double& property)
    {
        _serializedStr += std::to_string(property) + SEPARATOR;
    }
    inline void _Visit(const std::string& property)
    {
        _serializedStr += "'" + property + "'" + SEPARATOR;
    }


};

class IndexVisitor
{
public:
    size_t index;
    bool isFound;

    IndexVisitor(const void* pointer)
        : index(0), isFound(false), _pointer(pointer)
    { }

    template <typename ...Args>
    inline void Visit(Args& ...args)
    {
        return _Visit(args...);
    }

protected:
    const void* _pointer;

    template <typename First, typename ...Args>
    inline void _Visit(First& property, Args& ...args)
    {
        _Visit(property);
        if (!isFound)
            _Visit(args...);
    }

    template <typename T>
    inline void _Visit(const T &property)
    {
        if ((const void *)&property == _pointer)
            isFound = true;
        else
            index++;
    }

};

}


namespace ORMLite {

template <typename T>
class QueryMessager;

template <typename T>
class ORMapper
{
public:
    ORMapper(const std::string& dbName)
        : _dbName(dbName), _tableName(T::_CLASSNAME),
          _fieldsName(_Split(T::_FILEDSNAME))
    { }

    bool CreateTable()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            // 创建model类，使用访问者访问获得类中元信息
            const T modelObject {};
            ORMLite_Impl::TypeVisitor vistor;
            modelObject._Accept(vistor);

            auto fieldsType = _Split(vistor.GetSerializedStr());

            auto fieldsStr = _fieldsName[0] + " ORMLite_Impl::_Split" +
                    fieldsType[0] + " PRIMARY KEY NOT NULL,";

            for(auto i = 1; i < _fieldsName.size(); i++)
            {
                fieldsStr += _fieldsName[i] + " " +
                        fieldsType[i] + ",";
            }

            connector.Execute("CREATE TABLE " + _tableName +
                             " IF NOT EXISTS" + "(" + std::move(fieldsStr) + ");");
        });
    }

    bool DropTable()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            connector.Execute("DROP TABLE " + _tableName + ";");
        });
    }

    bool Insert(const T& modelObject)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            ORMLite_Impl::ReaderVisitor visitor;
            modelObject._Accept(visitor);

            connector.Execute("BEGIN TRANSACTION;" \
                              "INSERT INTO " + _tableName +
                              "VAULES (" + visitor.GetSerializedStr() + ");" +
                              "COMMIT TRANSACTION;");
        });
    }

    // 读入指定的对象，根据该对象的主键删除整行
    bool Delete(const T& modelObject)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            ORMLite_Impl::ReaderVisitor visitor;
            modelObject._Accept(visitor);

            // 格式如 "property = value"
            auto keyValueStr = _fieldsName[0] + "=" + visitor.GetSerializedStr();
            connector.Execute("DELETE FROM " + _tableName + " " +
                             "WHERE " + std::move(keyValueStr) + ";");
        });
    }

    // 删除满足propety=value的行
    bool Delete(const std::string& property, const std::string& value)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            connector.Execute("DELETE FROM " + _tableName + " " +
                              "WHERE " + property + "=" + value + ";");
        });
    }

    // 删除所有行
    bool DeleteAll()
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            connector.Execute("DELETE FROM " + _tableName + ";");
        });
    }

    // 根据指定的对象，更新相应的行
    bool Update(const T& modelObject)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            ORMLite_Impl::ReaderVisitor visitor;
            modelObject._Accept(visitor);

            auto fieldsValue = _Split(visitor.GetSerializedStr());
            auto keyStr = _fieldsName[0] + "=" + fieldsValue[0];
            auto fieldsStr = "";
            for(auto i = 1; i < _fieldsName.size(); i++)
            {
                fieldsStr += _fieldsName[i] + "=" +
                        fieldsValue[i] + ",";
            }

            connector.Execute("UPDATE " + _tableName +
                              "SET " + std::move(fieldsStr) +
                              "WHERE " + std::move(keyStr) + ";");
        });
    }

    // 设置key=keyValue那行的property为value
    bool Update(const std::string& keyValue, const std::string& property,
                const std::string& value)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            const T object {};
            ORMLite_Impl::ReaderVisitor visitor;
            object._Accept(visitor);

            auto key = _Split(visitor.GetSerializedStr())[0];

            connector.Execute("UPDATE " + _tableName +
                              "SET " + property + "=" + value +
                              "WHERE " + std::move(key) + "=" + keyValue + ";");
        });
    }

    // 查询返回的是由相应数据初始化的类型对象
    bool Query(const QueryMessager<T>& messager)
    {
        return _HandleException([&] (ORMLite_Impl::SQLConnector& connector)
        {
            auto sqlStr = "SELECT * FROM " + _tableName + " ";
            if (!messager._sqlWhere.empty())
                sqlStr += messager._sqlWhere;
            if (!messager._sqlLimit.empty())
                sqlStr += messager._sqlOrderBy;
            if (!messager._sqlOrderBy.empty())
                sqlStr += messager._sqlLimit;

            connector.Execute(sqlStr + ";", [&] (int column, char** columnText, char columnName)
            {
               std::vector<std::string> row;
               for (int i = 0; i < column; i++)
                   row.push_back(columnText[i]);

               messager.push_back(std::move(row));
            });
        });
    }


private:
    const std::string _dbName;
    const std::string _tableName;
    const std::vector<std::string> _fieldsName;
    std::string _errorMessage;

    bool _HandleException(std::function<void (ORMLite_Impl::SQLConnector& )> fn)
    {
        try
        {
            ORMLite_Impl::SQLConnector connector(_dbName);
            fn(connector);
            return true;
        }
        catch (const std::exception& e)
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

        for (const auto& ch : str)
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

template <typename T>
inline ORMLite_Impl::Field_Expr<T> Field(T& property)
{
    return ORMLite_Impl::Field_Expr<T>(property);
}

template <typename T>
class QueryMessager
{
    friend std::string ORMapper<T>::Query(const QueryMessager &messager);

public:
    QueryMessager(const T* pModelObject)
        : _pModelObject(pModelObject), _sqlWhere(""), _sqlOrderBy(""), _sqlLimit("")
    { }

    QueryMessager& Where(const ORMLite_Impl::Expr& expr)
    {
        for (const auto exprPair : expr.expr)
        {
            // first为nullptr说明为括号
            if (exprPair.first != nullptr)
                _sqlWhere += _GetFieldName(exprPair.first);
            _sqlWhere += exprPair.second;
        }

        return *this;
    }

    QueryMessager& OrderBy(const T& property, bool isDecreasing = false)
    {
        _sqlOrderBy = " ORDER BY " + _GetFieldName(property);
        if (isDecreasing)
            _sqlOrderBy += " DESC";

        return *this;
    }

    QueryMessager& Limit(size_t count, size_t offest)
    {
        _sqlLimit = " LIMIT " + std::to_string(count) +
                " OFFEST " + std::to_string(offest);

        return *this;
    }

    std::vector<std::vector<std::string>> GetVector()
    {
        return _result;
    }

    std::string GetField(const std::string& key, const void* property)
    {
        ORMLite_Impl::IndexVisitor visitor(property);
        _pModelObject->_Accept(visitor);

        for (auto row : _result)
        {
            if (row[0] == key)
                return row[visitor.index];
        }

        return "";
    }

private:
    const T* _pModelObject;
    std::string _sqlWhere;
    std::string _sqlOrderBy;
    std::string _sqlLimit;

    std::vector<std::vector<std::string>> _result;

    std::string _GetFieldName(const void* property)
    {
        ORMLite_Impl::IndexVisitor visitor;
        _pModelObject->_Accept(visitor);

        if (!visitor.isFound)
            throw std::runtime_error("No such field in the Table");

        return _Split(T::_FILEDSNAME)[visitor.index];
    }
};
}



#endif // ORMLITE_H
