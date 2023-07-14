#ifndef __EPOLLER_HPP
#define __EPOLLER_HPP

#include <vector>
#include <functional>
#include <map>

#include <sys/epoll.h>
#include <arpa/inet.h>

using epollTask = std::function<void()>;

class Epoller {
public:
    Epoller(int number, bool keepAlive);
    ~Epoller();

    void addFd(int fd, uint32_t events, const epollTask& task, const epollTask& quit, bool isListen = false);
    void addFd(int fd, uint32_t events, const epollTask& task, bool isListen = false);
    void modFd(int fd, uint32_t events, const epollTask& task);
    void modFd(int fd, uint32_t events);
    void modFd(int fd, const epollTask& task);
    void delFd(int fd);

    int wait(int timeout = -1);

    void run(int timeout = -1);

private:
    int epollFd_;
    int listenFd_;
    bool keepAlive_;
    std::vector<epoll_event> events_;
    std::map<int, epollTask> fdCallBack_;
    std::map<int, epollTask> quitCallBack_;
};

#endif //__EPOLLER_HPP
