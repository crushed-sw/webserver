#include "../include/user.hpp"

User::User(const std::string& username, const std::string& password) : username_(username), password_(password) {}

User::User(const std::vector<std::string>& vec) {
    username_ = vec[0];
    password_ = vec[1];
}

std::map<std::string, std::string> User::get() const {
    std::map<std::string, std::string> res;
    res["username"] = username_;
    res["password"] = password_;
    return res;
}

std::string User::parse() {
    std::stringstream ss;
    ss << "{\"username\":\"" << username_ << "\",\"password\":\"" << password_ << "\"}";
    return ss.str();
}

void User::setUsername(const std::string& username) {
    username_ = username;
}

void User::setPassword(const std::string& password) {
    password_ = password;
}

std::string User::getUsername() const {
    return username_;
}

std::string User::getPassword() const {
    return password_;
}
