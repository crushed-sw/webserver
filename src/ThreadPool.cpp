#include "../include/ThreadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool(size_t threadSize, size_t queueSize) : maxSize_(threadSize), queue_(queueSize), stop_(false) {
    for(int i = 0; i != threadSize; ++i) {
        workers_.emplace_back(std::make_shared<std::thread>(&ThreadPool::func, this));
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::stop() {
    std::call_once(flag_, [this]{ stopThread(); });
}

void ThreadPool::stopThread() {
    stop_ = true;
    queue_.stop();
    for(auto& worker : workers_)
        if(worker->joinable())
            worker->join();
}

void ThreadPool::func() {
    while(!stop_) {
        std::function<void()> task = {};
        queue_.take(task);
        if(task)
            task();
    }
}
