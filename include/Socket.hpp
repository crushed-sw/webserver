#ifndef __SOCKET_HPP
#define __SOCKET_HPP

#include "Epoller.hpp"
#include "ThreadPool.hpp"

#include <cstdint>
#include <functional>
#include <vector>

#include <sys/epoll.h>
#include <arpa/inet.h>

using socketTask = std::function<void(int, const sockaddr_in&)>;

class Socket {
public:
    enum Model {ReactorModel, ProactorModel};

    Socket(int port, int logBack, int threadnum, const Epoller& epoller, uint32_t listenEvent = EPOLLET, uint32_t connEvent = EPOLLET);
    ~Socket();

    void start(Model model = ReactorModel);

    void setOnConnection(const socketTask& task);
    void setOnMessage(const socketTask& task);
    void setOnQuit(const socketTask& task);

    void setReuseAddr();
    void setReusePort();
    void setKeepAlive();

private:
    void setNonBlock(int fd);
    void bind();
    void listen();

    void reactorAccept(int fd, const sockaddr_in& addr);
    void proactorAccept(int fd, const sockaddr_in& addr);
    void onConnection(int fd, const sockaddr_in& addr);
    void onMessage(int fd, const sockaddr_in& addr);
    void onQuit(int fd, const sockaddr_in& addr);

    int port_;
    int listenFd_;
    int logBack_;
    uint32_t listenEvent_;
    uint32_t connEvent_;
    sockaddr_in address_;
    Epoller epoller_;

    socketTask connection_;
    socketTask message_;
    socketTask quit_;
    epollTask accept_;

    ThreadPool threadpool;
};

#endif //__SOCKET_HPP
