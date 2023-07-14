#ifndef __REQUEST_HPP
#define __REQUEST_HPP

#include <string>
#include <map>

#include "Buffer.hpp"

class Request {
public:
    enum Method {GET, POST, PUT, DELETE, NONE};

    Request(Buffer& buffer);
    
    Request::Method stringToMethod(const std::string& str);
    Request::Method getMethod();
    std::string getPath();
    std::string getVersion();
    std::map<std::string, std::string> getHeader();
    std::map<std::string, std::string> getParameter();

private:
    void parseRequest(std::string& str);
    void parseHeader(std::string& str);
    void parseParameter(std::string& str, char c);
    void parseGetParameter();
    void clearStream();

    std::string method_;
    std::string path_;
    std::string version_;

    std::stringstream ss_;

    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> parameters_;
};

#endif //__REQUEST_HPP
