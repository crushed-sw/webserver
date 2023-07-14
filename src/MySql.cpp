#include "../include/MySql.hpp"

#include <mysql/mysql.h>
#include <stdexcept>
#include <memory>

MySql::MySql(const std::string& host, const std::string& user, const std::string& password, const std::string& database) : 
            host_(host), user_(user), password_(password), mysql_(mysql_init(nullptr)), database_(database) {
    mysql_real_connect(mysql_, host_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), 0, nullptr, 0);
}

MySql::~MySql() {
    mysql_close(mysql_);
}

void MySql::setDatabase(const std::string& str) {
    database_ = str;
    change();
}

void MySql::setTable(const std::string& str) {
    table_ = str;
}

void MySql::change() {
    mysql_close(mysql_);
    mysql_real_connect(mysql_, host_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), 0, nullptr, 0);
}

MySql& MySql::prepare(const std::string& query) {
    stmt_ = mysql_stmt_init(mysql_);
    if(!stmt_)
        throw std::runtime_error("mysql_stmt_init() error");

    if(mysql_stmt_prepare(stmt_, query.c_str(), query.length()))
        throw std::runtime_error("mysql_stmt_prepare() error");

    return *this;
}

MySql& MySql::bindParameter(const std::vector<paraType>& para) {
    for(int i = 0; i != para.size(); ++i)
        getVariant(para[i], parameter_[i]);
    mysql_stmt_bind_param(stmt_, parameter_);
    return *this;
}

MySql& MySql::execute() {
    if(mysql_stmt_execute(stmt_))
        throw std::runtime_error("mysql_stmt_execute() error");
    return *this;
}

std::vector<std::vector<std::string>> MySql::getResult() {
    result_ = mysql_stmt_result_metadata(stmt_);

    int fieldNum = mysql_num_fields(result_);

    std::vector<std::vector<std::string>> res;

    MYSQL_ROW row = {};
    while((row = mysql_fetch_row(result_)) != nullptr) {
        std::vector<std::string> vec;
        for(int i = 0; i != fieldNum; ++i) {
            vec.emplace_back(row[i]);
        }
        res.push_back(vec);
    }

    mysql_free_result(result_);
    mysql_stmt_close(stmt_);

    return res;
}

void MySql::getVariant(const paraType& para, MYSQL_BIND& bind) {
    if(auto value = std::get_if<int>(&para)) {
        bind.buffer_type = MYSQL_TYPE_LONG;
        bind.buffer = const_cast<void*>(static_cast<const void*>(value));
    } else if(auto value = std::get_if<long long>(&para)) {
        bind.buffer_type = MYSQL_TYPE_LONGLONG;
        bind.buffer = const_cast<void*>(static_cast<const void*>(value));
    } else if(auto value = std::get_if<float>(&para)) {
        bind.buffer_type = MYSQL_TYPE_FLOAT;
        bind.buffer = const_cast<void*>(static_cast<const void*>(value));
    } else if(auto value = std::get_if<double>(&para)) {
        bind.buffer_type = MYSQL_TYPE_DOUBLE;
        bind.buffer = const_cast<void*>(static_cast<const void*>(value));
    } else if(auto value = std::get_if<std::string>(&para)) {
        bind.buffer_type = MYSQL_TYPE_STRING;
        bind.buffer_length = value->length();
        bind.buffer = const_cast<void*>(static_cast<const void*>(value));
    } else {
        throw std::runtime_error("parameter type error");
    }
}
