#include "../include/Response.hpp"

#include <sys/socket.h>

Response::Response(const std::string& version, int code, const std::string& str) : version_(version), code_(code), state_(str), change_(true) {}

void Response::setCode(int code, const std::string& str) {
    code_ = code;
    state_ = str;
    change_ = true;
}

void Response::setVersion(const std::string& str) {
    version_ = str;
    change_ = true;
}

void Response::setHeader(const std::string& key, const std::string& value) {
    header_[key] = value;
    change_ = true;
}

void Response::setContent(const std::string& str) {
    content_ = str;
    change_ = true;
}

void Response::addContent(const std::string& str) {
    content_ += str;
    change_ = true;
}

void Response::send(int fd) {
    std::string str = getMessage();
    ::send(fd, str.c_str(), str.length(), 0);
}

std::string Response::getMessage() {
    if(change_) {
        message_.str("");
        message_.clear();
        message_ << version_ << " " << code_ << " " << state_ << "\n";
        for(auto header : header_)
            message_ << header.first << ":" << header.second << "\n";
        message_ << "\n" << content_;
    }
    return message_.str();
}
