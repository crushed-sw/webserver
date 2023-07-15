#include "../include/MySqlPool.hpp"

MySqlPool::MySqlPool(const std::string& url, const std::string& user, const std::string& password, int poolSize)
            : url_(url), user_(user), password_(password), poolSize_(poolSize), pool_(poolSize) {
    for(int i = 0; i != poolSize; ++i) {
        MySql* mysql = new MySql(url_, user_, password_);
        pool_.put(mysql);
    }
}

MySqlPool::~MySqlPool() {
    pool_.stop();
    for(int i = 0; i != poolSize_; ++i) {
        MySql* mysql = nullptr;
        pool_.take(mysql);
        if(mysql)
            delete mysql;
    }
}

MySql* MySqlPool::getConnection() {
    MySql* mysql = nullptr;
    pool_.take(mysql);
    return mysql;
}

MySql* MySqlPool::getConnection(const std::string& table) {
    MySql* mysql = nullptr;
    pool_.take(mysql);
    mysql->setTable(table);
    return mysql;
}

void MySqlPool::releaseConnection(MySql* mysql) {
    pool_.put(mysql);
}
