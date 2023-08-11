#include "../include/userService.hpp"

User UserService::query(const std::string& str) {
    return userMapping_.query(str);
}

void UserService::insert(const User& user) {
    userMapping_.insert(user);
}
