#ifndef __USER_MAPPING_HPP
#define __USER_MAPPING_HPP

#include "../../../include/MySqlPool.hpp"
#include "../../../include/MySqlConn.hpp"
#include "../../entity/include/user.hpp"

class UserMapping {
public:
    User query(const std::string& str);
    void insert(const User& user);

private:
    MySql* getMysql();
};

#endif // __USER_MAPPING_HPP
