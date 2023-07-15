#include "include/MySqlConn.hpp"

#include <iostream>

class User {
public:
    User(const std::string& username, const std::string& password) : username(username), password(password) {}

    User(const std::vector<std::string>& vec) {
        if(vec.size() < 2)
            throw std::runtime_error("vec error");
        username = vec[0];
        password = vec[1];
    }

    std::map<std::string, std::string> get() const {
        std::map<std::string, std::string> res;
        res["username"] = username;
        res["password"] = password;
        return res;
    }

    std::string username;
    std::string password;
};

int main() {
    MySqlPool mysqlPool("tcp://localhost:3306/webserver", "root", "zk200111", 5);
    MySql* mysql = nullptr;

    MySqlConn conn(mysql, mysqlPool, "user");

    auto users = mysql->queryAll<User>();
    for(auto u : users) {
        std::cout << u.username << " " << u.password << std::endl;
    }
}
