#ifndef __MY_SQL_HPP
#define __MY_SQL_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <mutex>
#include <string>

#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

class MySql {
public:
    MySql(const std::string& url, const std::string& user, const std::string& password);
    MySql(const std::string& url, const std::string& user, const std::string& password, const std::string& table);

    void connect();

    void setTable(const std::string& table);
    void setDatabse(const std::string& database);

    MySql& query(const std::string& str);
    MySql& update(const std::string& str);
    MySql& bind(const std::vector<std::string>& para);

    sql::ResultSet* executeQuery();
    void executeUpdate();

    template <typename T>
    std::vector<T> executeQuery();

    template <typename T>
    T executeQueryOne();

    template <typename T>
    void updateById(T& item);

    template <typename T>
    void updateById(T& item, const std::string& id, const std::string& idValue);

    template <typename T>
    void insert(T& item);

    template <typename T>
    void insert(const std::vector<T>& items);

    template <typename T>
    void deleteById(T& item);

    template <typename T>
    void deleteById(T& item, const std::string& id, const std::string& idValue);

    template <typename T>
    T queryById(long long id);

    template <typename T>
    T queryById(long long id, const std::string& idName);

    template <typename T>
    std::vector<T> queryAll();
    
private:
    template <typename T>
    void getPara(std::string& query, std::vector<std::string>& para, const T& item, const std::string& id, const std::string& value);

    std::string url_;
    std::string user_;
    std::string password_;
    std::string table_;
    std::string database_;
    sql::Driver* driver_;
    std::unique_ptr<sql::Connection> conn_;
    std::unique_ptr<sql::PreparedStatement> pstmt_;
    std::mutex mutex_;
};

template <typename T>
std::vector<T> MySql::executeQuery() {
    std::unique_ptr<sql::ResultSet> res(executeQuery());
    std::vector<T> ans;

    int columns =  res->getMetaData()->getColumnCount();
    while(res->next()) {
        std::vector<std::string> vec;
        for(int i = 1; i <= columns; ++i)
            vec.emplace_back(res->getString(i));
        ans.emplace_back(vec);
    }
    return ans;
}

template <typename T>
T MySql::executeQueryOne() {
    std::unique_ptr<sql::ResultSet> res(executeQuery());
    std::vector<std::string> vec;

    int columns =  res->getMetaData()->getColumnCount();
    if(res->next()) {
        for(int i = 1; i <= columns; ++i)
            vec.emplace_back(res->getString(i));
    }
    return T(vec);
}

template <typename T>
void MySql::updateById(T& item) {
    std::map<std::string, std::string> name = item.get();
    updateById<T>(item, "id", name["id"]);
}

template <typename T>
void MySql::updateById(T& item, const std::string& id, const std::string& idValue) {
    std::stringstream ss;
    ss << "update " << table_ << " set ";
    std::map<std::string, std::string> name = item.get();

    std::vector<std::string> vec;
    for(auto kv : name) {
        ss << kv.first << " = ? ";
        vec.push_back(kv.second);
    }

    ss << " where " << id << " = ? ";
    vec.push_back(idValue);

    query(ss.str()).bind(vec).executeUpdate();
}

template <typename T>
void MySql::insert(const std::vector<T>& items) {
    for(T& item : items)
        insert(item);
}
template <typename T>
void MySql::insert(T& item) {
    std::stringstream ss;
    ss << "insert into " << table_ << " (";
    std::map<std::string, std::string> name = item.get();

    bool is = false;

    std::vector<std::string> vec;

    int index = name.size();
    for(auto kv : name) {
        if(kv.first == "id") {
            is = true;
            continue;
        }
        if(--index == 0)
            ss << kv.first;
        else
            ss << kv.first << ", ";
        vec.push_back(kv.second);
    }
    ss << ") values (";

    int num = name.size() - is;

    for(int i = 0; i != num; ++i) {
        if(i == 0)
            ss << "?";
        else
            ss << ", ?";
    }
    ss << ")";

    std::cout << ss.str() << std::endl;

    query(ss.str()).bind(vec).executeUpdate();
}

template <typename T>
void MySql::deleteById(T& item) {
    std::map<std::string, std::string> name = item.get();
    deleteById<T>(item, "id", name["id"]);
}

template <typename T>
void MySql::deleteById(T& item, const std::string& id, const std::string& idValue) {
    std::stringstream ss;
    ss << "delete from " << table_;
    std::map<std::string, std::string> name = item.get();

    ss << " where " << id << " = ? ";

    query(ss.str()).bind({idValue}).executeUpdate();
}

template <typename T>
T MySql::queryById(long long id) {
    return queryById<T>(id, "id");
}

template <typename T>
T MySql::queryById(long long id, const std::string& idName) {
    std::stringstream ss;
    ss << "select * from " << table_ << " where " << idName << " = ? ";
    return query(ss.str()).bind({idName}).executeQueryOne<T>();
}

template <typename T>
std::vector<T> MySql::queryAll() {
    std::stringstream ss;
    ss << "select * from " << table_;
    return query(ss.str()).executeQuery<T>();
}

#endif //__MY_SQL_HPP
