#ifndef __SERVER_CONFIG_HPP
#define __SERVER_CONFIG_HPP

#include "Log.hpp"

#include <fstream>
#include <unordered_map>
#include <string>

class ServerConfig {
public:
    void loadConfigFile(const std::string& fileName);
    std::string getValue(const std::string& str) const;
    
private:
    void trim(std::string& str);

    std::unordered_map<std::string, std::string> map_;
};

#endif //__SERVER_CONFIG_HPP
