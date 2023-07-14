#include "../include/Request.hpp"

#include <cstring>
#include <algorithm>

Request::Request(Buffer& buffer) {
    std::string str = buffer.getLine();
    parseHeader(str);
    
    while(true) {
        str = buffer.getLine();
        if(str.empty() || str[0] == '\n')
            break;
        parseRequest(str);
    }

    if(method_ == "GET") {
        parseGetParameter();
    } else {
        while(true) {
            str = buffer.getLine();
            if(str.empty() || str[0] == '\n')
                break;
            parseParameter(str, ':');
        }
    }
}

void Request::parseHeader(std::string& str) {
    clearStream();
    std::stringstream ss(str);
    ss >> method_ >> path_ >> version_;
}

void Request::parseRequest(std::string& str) {
    clearStream();

    auto iter = std::find(str.begin(), str.end(), ':');
    if(iter != str.end())
        *iter = ' ';

    ss_.str(str);
    std::string key = {};
    std::string value = {};
    ss_ >> key >> value;

    if(!key.empty() && !value.empty())
        headers_[key] = value;
}

void Request::parseParameter(std::string& str, char c) {
    clearStream();

    auto iter = std::find(str.begin(), str.end(), c);
    if(iter != str.end())
        *iter = ' ';

    ss_.str(str);
    std::string key = {};
    std::string value = {};
    ss_ >> key >> value;

    if(!key.empty() && !value.empty())
        parameters_[key] = value;
}

void Request::parseGetParameter() {
    clearStream();

    auto iter = std::find(path_.begin(), path_.end(), '?');
    if(iter != path_.end())
        *iter = ' ';
        
    ss_.str(path_);
    std::string parameter = {};
    ss_ >> path_ >> parameter;

    std::stringstream ss(parameter);
    std::string kv = {};
    while(std::getline(ss, kv, '&'))
        parseParameter(kv, '=');
}

void Request::clearStream() {
    ss_.str("");
    ss_.clear();
}

Request::Method Request::stringToMethod(const std::string& str) {
    if(str == "GET") {
        return GET;
    } else if(str == "POST") {
        return POST;
    } else if(str == "PUT") {
        return PUT;
    } else if(str == "DELETE") {
        return DELETE;
    }
    return NONE;
}

Request::Method Request::getMethod() {
    return stringToMethod(method_);
}

std::string Request::getPath() {
    return path_;
}

std::string Request::getVersion() {
    return version_;
}

std::map<std::string, std::string> Request::getHeader() {
    return headers_;
}

std::map<std::string, std::string> Request::getParameter() {
    return parameters_;
}
