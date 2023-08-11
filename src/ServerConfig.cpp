#include "../include/ServerConfig.hpp"

void ServerConfig::loadConfigFile(const std::string& fileName) {
    std::fstream fs(fileName);
    if(!fs.is_open()) {
        Error << "file opern error";
        exit(EXIT_FAILURE);
    }

    std::string str;
    while(std::getline(fs, str)) {
        trim(str);
        if(str.empty() || str[0] == '#') {
            continue;
        }

        int index = str.find('=');
        std::string key = str.substr(0, index);
        std::string value = str.substr(index + 1, str.size() - index);
        trim(key);
        trim(value);

        map_.insert({key, value});
    }
}

std::string ServerConfig::getValue(const std::string& str) const {
    auto iter = map_.find(str);
    if(iter == map_.end()) {
        return "";
    }
    return iter->second;
}

void ServerConfig::trim(std::string& str) {
    int index = str.find_first_not_of(' ');
    if(index != -1) {
        str = str.substr(index, str.size() - index);
    }

    index = str.find_last_not_of(' ');
    if(index != -1) {
        str = str.substr(0, index + 1);
    }
}
