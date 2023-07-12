#include "../include/Epoller.hpp"

#include <stdexcept>

#include <unistd.h>

Epoller::Epoller(int number) {
    if(number <= 0)
        throw std::runtime_error("Epoller(int number) the number must larger than zero");

    epollFd_ = epoll_create(number);
    events_.resize(number);
}

Epoller::~Epoller() {
    close(epollFd_);
    events_.clear();
    fdCallBack_.clear();
}

void Epoller::addFd(int fd, uint32_t events, const epollTask& task) {
    epoll_event event = {};
    event.data.fd = fd;
    event.events = events;

    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0)
        throw std::runtime_error("epoll_ctl() to addFd error");

    fdCallBack_[fd] = task;
}

void Epoller::modFd(int fd, uint32_t events, const epollTask& task) {
    modFd(fd, events);
    modFd(fd, task);
}

void Epoller::modFd(int fd, uint32_t events) {
    epoll_event event = {};
    event.data.fd = fd;
    event.events = events;

    if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0)
        throw std::runtime_error("epoll_ctl() to modFd error");
}

void Epoller::modFd(int fd, const epollTask& task) {
    fdCallBack_[fd] = task;
    fdCallBack_.erase(fd);
}

void Epoller::delFd(int fd) {
    if(epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr) < 0)
        throw std::runtime_error("epoll_ctl() to delFd error");
}

int Epoller::wait(int timeout) {
    int ret = epoll_wait(epollFd_, events_.data(), events_.size(), timeout);
    if(ret < 0)
        throw std::runtime_error("epoll_wait() error");
    return ret;
}

void Epoller::run(int timeout) {
    while(true) {
        int ret = wait(timeout);
        for(int i = 0; i != ret; ++i) {
            int sockfd = events_[i].data.fd;
            uint32_t events = events_[i].events;

            auto iter = fdCallBack_.find(sockfd);
            if(iter == fdCallBack_.end())
                throw std::runtime_error("Epoller::fdCallBack error");
            else
                iter->second(sockfd, events);
        }
    }
}
