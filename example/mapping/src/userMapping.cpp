#include "../include/userMapping.hpp"

User UserMapping::query(const std::string& str) {
    MySql* mysql = getMysql();
    std::stringstream ss;
    ss << "select * from user where username = ? ";
    return mysql->query(ss.str()).bind({str}).executeQueryOne<User>();
}

void UserMapping::insert(const User& user) {
    MySql* mysql = getMysql();
    mysql->insert(user);
}

MySql* UserMapping::getMysql() {
    MySql* mysql = nullptr;
    MySqlPool& pool = MySqlPool::getInstance();
    MySqlConn conn(mysql, pool, "user");
    return mysql;
}
