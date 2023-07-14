#ifndef __MY_SQL_HPP
#define __MY_SQL_HPP

#include <string>
#include <vector>
#include <variant>


#include <mysql/mysql.h>

using paraType = std::variant<int, long long, float, double, std::string>;

class MySql {
public:
    MySql(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    ~MySql();

    void setDatabase(const std::string& str);
    void setTable(const std::string& str);

    MySql& prepare(const std::string& str);
    MySql& bindParameter(const std::vector<paraType>& para);
    MySql& execute();
    std::vector<std::vector<std::string>> getResult();
 
private:
   void getVariant(const paraType& para, MYSQL_BIND& bind);

    void change();

    MYSQL* mysql_;
    MYSQL_RES* result_;
    MYSQL_STMT* stmt_;
    MYSQL_BIND parameter_[64];

    std::string database_;
    std::string table_;
    std::string host_;
    std::string user_;
    std::string password_;
};

#endif //__MY_SQL_HPP
