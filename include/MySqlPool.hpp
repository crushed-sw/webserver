#ifndef __MY_SQL_POOL_HPP
#define __MY_SQL_POOL_HPP

#include <string>
#include <cstdlib>

#include "Log.hpp"
#include "ServerApplication.hpp"
#include "ServerConfig.hpp"
#include "SyncQueue.hpp"
#include "MySql.hpp"

class MySqlPool {
public:
    ~MySqlPool();

    static MySqlPool& getInstance();

    MySql* getConnection();
    MySql* getConnection(const std::string& table);
    void releaseConnection(MySql* mysql);

private:
    MySqlPool();

    MySqlPool(const MySqlPool&) = delete;
    MySqlPool(MySqlPool&&) = delete;
    MySqlPool operator=(const MySqlPool&) = delete;
    MySqlPool  operator=(MySqlPool&&) = delete;

    std::string url_;
    std::string user_;
    std::string password_;
    int poolSize_;
    SyncQueue<MySql*> pool_;
};

#endif //__MY_SQL_POOL_HPP
