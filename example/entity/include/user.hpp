#ifndef __USER_HPP
#define __USER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <map>

class User {
public:
    User(const std::string& username = "", const std::string& password = "");
    User(const std::vector<std::string>& vec);
    std::map<std::string, std::string> get() const;
    std::string parse();

    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    std::string getUsername() const;
    std::string getPassword() const;

private:
    std::string username_;
    std::string password_;
};

#endif //__USER_HPP
