#ifndef __MY_SQL_POOL_HPP
#define __MY_SQL_POOL_HPP

#include <string>

#include "SyncQueue.hpp"
#include "MySql.hpp"

class MySqlPool {
public:
    MySqlPool(const std::string& url, const std::string& user, const std::string& password, int poolSize);
    ~MySqlPool();

    MySql* getConnection();
    MySql* getConnection(const std::string& table);
    void releaseConnection(MySql* mysql);

private:
    std::string url_;
    std::string user_;
    std::string password_;
    int poolSize_;
    SyncQueue<MySql*> pool_;
};

#endif //__MY_SQL_POOL_HPP
