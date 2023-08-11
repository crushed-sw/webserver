#ifndef __USER_SERVICE_HPP
#define __USER_SERVICE_HPP

#include "../../mapping/include/userMapping.hpp"

class UserService {
public:
    User query(const std::string& str);
    void insert(const User& user);

private:
    UserMapping userMapping_;
};

#endif //__USER_SERVICE_HPP
