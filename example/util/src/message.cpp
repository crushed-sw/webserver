#include "../include/message.hpp"

Message<std::string>::Message(int code, std::string data) {
    code_ = code;
    data_ = data;
}

std::string Message<std::string>::parse() {
    std::stringstream ss;
    ss << "{\"code\":" << code_ << ",\"data\":\"" << data_ << "\"}";
    return ss.str();
}
