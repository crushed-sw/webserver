#ifndef __HTTP_SERVER_HPP
#define __HTTP_SERVER_HPP

#include "ServerApplication.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

#include <string>
#include <fstream>
#include <functional>

using httpTask = std::function<std::string(Request& request, Response& response)>;

class HttpServer {
public:
    HttpServer(int port, int threadNum, uint32_t listenEvent, uint32_t connEvent, Socket::Model model);
    HttpServer();

    void setResourceDir(const std::string& str);
    void addInterceptor(const httpTask& interceptor);

    void setCallBack(const std::string& path, Request::Method method, const httpTask& callBack);

    void start();

    static const int MAX_RECV_LEN = 2048;

private:
    void notFound(Response& response);

    void connection(int fd, const sockaddr_in& addr);
    void message(int fd, const sockaddr_in& addr);
    void quit(int fd, const sockaddr_in& addr);
    void logInit();

    void getAllFile(const std::string& path);

    int port_;
    int threadNum_;
    uint32_t listenEvent_;
    uint32_t connEvent_;
    Socket::Model model_;

    char buffer_[MAX_RECV_LEN];
    Buffer buf_;

    std::string resource_;
    std::map<std::string, std::string> files_;
    std::vector<httpTask> interceptor_;
    std::map<std::string, std::map<Request::Method, httpTask>> callBack_;
    std::map<int, std::string> ipAddress_;
};

#endif //__HTTP_SERVER_HPP
