#ifndef __MESSAGE_HPP
#define __MESSAGE_HPP

#include <string>
#include <sstream>

template <typename T>
class Message {
public:
    Message(int code, T data);
    std::string parse();

private:
    int code_;
    T data_;
};

template <>
class Message<std::string> {
public:
    Message(int code, std::string data);
    std::string parse();

private:
    int code_;
    std::string data_;
};


template <typename T>
Message<T>::Message(int code, T data) {
    code_ = code;
    data_ = data;
}

template <typename T>
std::string Message<T>::parse() {
    std::stringstream ss;
    ss << "{\"code\":" << code_ << ",\"data\":" << data_.parse() << "}";
    return ss.str();
}

#endif //__MESSAGE_HPP
