#ifndef __RESPONSE_HPP
#define __RESPONSE_HPP

#include <string>
#include <sstream>
#include <map>

class Response {
public:
    Response(const std::string& version = "HTTP/1.1", int code = 200, const std::string& str = "OK");

    void setVersion(const std::string& str);
    void setCode(int code, const std::string& str);
    void setHeader(const std::string& key, const std::string& value);
    void addContent(const std::string& str);
    void setContent(const std::string& str);

    void send(int fd);
    std::string getMessage();

private:
    int code_;
    bool change_;
    std::string state_;
    std::string version_;
    std::string content_;
    std::stringstream message_;
    std::map<std::string, std::string> header_;
};

#endif //__RESPONSE_HPP
