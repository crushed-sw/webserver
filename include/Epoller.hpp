#ifndef __EPOLLER_HPP
#define __EPOLLER_HPP

#include <vector>
#include <functional>
#include <map>

#include <sys/epoll.h>

using epollTask = std::function<void(int, int)>;

class Epoller {
public:
    Epoller(int number);
    ~Epoller();

    void addFd(int fd, uint32_t events, const epollTask& task);
    void modFd(int fd, uint32_t events, const epollTask& task);
    void modFd(int fd, uint32_t events);
    void modFd(int fd, const epollTask& task);
    void delFd(int fd);

    int wait(int timeout = -1);

    void run(int timeout = -1);

private:
    int epollFd_;
    std::vector<epoll_event> events_;
    std::map<int, epollTask> fdCallBack_;
};

#endif //__EPOLLER_HPP
