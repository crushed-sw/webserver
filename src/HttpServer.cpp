#include "../include/HttpServer.hpp"
#include "../include/Socket.hpp"
#include "../include/Log.hpp"

#include <unistd.h>
#include <cstring>

#include <sys/stat.h>
#include <dirent.h>

using namespace std::placeholders;

HttpServer::HttpServer(int port, int threadNum, uint32_t listenEvent,  uint32_t connEvent, Socket::Model model) : 
                        port_(port), threadNum_(threadNum), listenEvent_(listenEvent), connEvent_(connEvent), model_(model) {
    char buf[512] = {};
    getcwd(buf, sizeof(buf));
    resource_ = std::string(buf, strlen(buf)) + "/resource";
}

void HttpServer::setResourceDir(const std::string& str) {
    resource_ = str;
}

void HttpServer::addInterceptor(const httpTask& task) {
    interceptor_.push_back(task);
}

void HttpServer::setCallBack(const std::string& path, Request::Method method, const httpTask& callBack) {
    callBack_[path][method] = callBack;
}

void HttpServer::start() {
    Epoller epoller(10, false);
    Socket socket(port_, 10, threadNum_, epoller, listenEvent_, connEvent_);

    logInit();

    getAllFile(resource_);

    socket.setOnConnection(std::bind(&HttpServer::connection, this, _1, _2));
    socket.setOnMessage(std::bind(&HttpServer::message, this, _1, _2));
    socket.setOnQuit(std::bind(&HttpServer::quit, this, _1, _2));

    socket.setKeepAlive();
    socket.setReusePort();

    socket.start(model_);
}

void HttpServer::getAllFile(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if(dir == nullptr)
        throw std::runtime_error("resource dir can't open");

    dirent* entry = nullptr;
    while((entry = readdir(dir)) != nullptr) {
        if(entry->d_name[0] == '.')
            continue;

        std::string filePath = path + "/" + entry->d_name;
        struct stat st = {};
        if(stat(filePath.c_str(), &st) == -1)
            continue;

        if(S_ISDIR(st.st_mode)) {
            getAllFile(filePath);
        } else if(S_ISREG(st.st_mode)) {
            buf_.clear();
            buf_ << std::fstream(filePath);
            files_[filePath] = buf_.getAll();
        }
    }

    closedir(dir);
}

void HttpServer::connection(int fd, const sockaddr_in& addr) {
    char buf[INET_ADDRSTRLEN];
    char ip[30];
    sprintf(ip, "%s:%d", inet_ntop(addr.sin_family, &addr.sin_addr, buf, INET_ADDRSTRLEN), ntohs(addr.sin_port));
    ipAddress_[fd] = std::string(ip, strlen(ip));
}

void HttpServer::message(int fd, const sockaddr_in& addr) {
    int len = recv(fd, buffer_, MAX_RECV_LEN, 0);
    Buffer buffer(std::move(std::string(buffer_, len)));

    Request request(buffer);
    Response response;

    for(auto interceptor : interceptor_)
        interceptor(request, response);

    std::string path = request.getPath();
    if(path.back() == '/')
        path += "index.html";
    Request::Method method = request.getMethod();

    bool is = true;

    auto iter = files_.find(resource_ + path);
    if(is && iter != files_.end()) {
        buf_.clear();
        buf_ << files_[resource_ + path];
        response.setContent(buf_.getAll());
        is = false;
    }

    auto pathIter = callBack_.find(path);
    if(is && pathIter != callBack_.end()) {
        auto methodIter = pathIter->second.find(method);
        if(methodIter != pathIter->second.end()) {
            response.addContent(callBack_[path][method](request, response));
            is = false;
        }
    }

    if(is)
        notFound(response);

    response.send(fd);
}

void HttpServer::notFound(Response& response) {
    response.setCode(404, "Not Found");
    bool is = true;
    
    auto iter = files_.find(resource_ + "/notFound.html");
    if(is && iter != files_.end()) {
        buf_.clear();
        buf_ << iter->second;
        response.setContent(buf_.getAll());
        is = false;
    }

    if(is) {
        buf_.clear();
        buf_ << "<!DOCTYPE html><html><head><title>404 Not Found</title><style>"
             << "body {font-family: Arial, sans-serif;background-color: #f1f1f1;}"
		     << ".container {margin: 50px auto;width: 80%;max-width: 800px;text-align: center;}"
		     << "h1 {font-size: 5em;color: #999;margin: 0;}"
             << "p {font-size: 1.5em;color: #666;margin: 0;}"
             << "</style></head><body><div class='container'><h1>404</h1>"
             << "<p>Sorry, the page you requested could not be found.</p>"
             << "</div></body></html>";
        response.setContent(buf_.getAll());
    }
}

void HttpServer::quit(int fd, const sockaddr_in& addr) {}

void HttpServer::logInit() {
    std::stringstream ss;
    ss << "========== Server Init ==========\n";
    ss << "Port: " << port_ << ", threadNum: " << threadNum_ << "\n";
    ss << "ListenEvent: " << ((listenEvent_ & EPOLLET) ? "ET" : "LT") << ", ConnectEvent: " << ((connEvent_ & EPOLLET) ? "ET" : "LT") << "\n";
    ss << "Resource Directory: " << resource_ << "\n";
    ss << "========== Server Start ==========\n";

    std::string str;
    while(std::getline(ss, str))
        Info << str;
}
