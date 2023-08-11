#include "../include/MySqlPool.hpp"

MySqlPool::MySqlPool() {
    ServerConfig& conf = ServerApplication::getInstance().getConfig();
    url_ = conf.getValue("sql_url");
    user_ = conf.getValue("sql_user");
    password_ = conf.getValue("sql_password");
    std::string poolsize = conf.getValue("sql_poolsize").c_str();
    
    if(url_ == "" || user_ == "" || password_ == "") {
        Error << "mysql config error";
        exit(EXIT_FAILURE);
    }

    if(poolsize == "") {
        poolsize = "5";
    }

    poolSize_ = atoi(poolsize.c_str());
    pool_.setNum(poolSize_);

    for(int i = 0; i != poolSize_; ++i) {
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

MySqlPool& MySqlPool::getInstance() {
    static MySqlPool pool;
    return pool;
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
