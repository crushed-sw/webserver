#include "../include/Socket.hpp"

#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>

using namespace std::placeholders;

Socket::Socket(int port, int logBack, int num, const Epoller& epoller, uint32_t listenEvent, uint32_t connEvent) : 
               port_(port), logBack_(logBack), threadpool(num, num * 50), epoller_(epoller), listenEvent_(listenEvent), connEvent_(connEvent) {
    listenFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(listenFd_ < 0)
        throw std::runtime_error("socket() error");

    connection_ = std::bind(&Socket::onConnection, this, _1, _2);
    message_ = std::bind(&Socket::onMessage, this, _1, _2);
}

Socket::~Socket() {
    close(listenFd_);
}

void Socket::start(Model model) {
    bind();
    listen();

    if(model == Socket::ProactorModel) {
        accept_ = std::bind(&Socket::proactorAccept, this, listenFd_, address_);
    } else {
        accept_ = std::bind(&Socket::reactorAccept, this, listenFd_, address_);
    }

    epoller_.addFd(listenFd_, EPOLLIN | listenEvent_, accept_, true);
    epoller_.run();
}

void Socket::setOnConnection(const socketTask& task) {
    connection_ = task;
}

void Socket::setOnMessage(const socketTask& task) {
    message_ = task;
}

void Socket::setOnQuit(const socketTask& task) {
    quit_ = task;
}

void Socket::setReuseAddr() {
    int optVal = 1;
    int ret = setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if(ret < 0)
        throw std::runtime_error("setsockopt() to setReuseAddr error");
}

void Socket::setReusePort() {
    int optVal = 1;
    int ret = setsockopt(listenFd_, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));
    if(ret < 0)
        throw std::runtime_error("setsockopt() to setReusePort error");
}

void Socket::setKeepAlive() {
    int optVal = 1;
    int ret = setsockopt(listenFd_, SOL_SOCKET, SO_KEEPALIVE, &optVal, sizeof(optVal));
    if(ret < 0)
        throw std::runtime_error("setsockopt() to setKeepAlive error");
}

void Socket::setNonBlock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::bind() {
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(listenFd_, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    if(ret < 0)
        throw std::runtime_error("bind error");

    address_ = address;
}

void Socket::listen() {
    int ret = ::listen(listenFd_, logBack_);
    if(ret < 0)
        throw std::runtime_error("listen error");
}

void Socket::reactorAccept(int fd, const sockaddr_in& addr) {
    sockaddr_in client = {};
    socklen_t clientLen = sizeof(client);
    int connfd = ::accept(fd, reinterpret_cast<sockaddr*>(&client), &clientLen);
    if(connfd < 0)
        throw std::runtime_error("accept() error");
    connection_(connfd, client);

    epoller_.addFd(connfd, EPOLLIN | connEvent_, std::bind(message_, connfd, client), std::bind(quit_, connfd, client));
}

void Socket::proactorAccept(int fd, const sockaddr_in& addr) {
    reactorAccept(fd, addr);
    threadpool.submit(std::bind(message_, fd, addr));
}

void Socket::onConnection(int fd, const sockaddr_in& addr) {}

void Socket::onMessage(int fd, const sockaddr_in& addr) {}

void Socket::onQuit(int fd, const sockaddr_in& addr) {}

