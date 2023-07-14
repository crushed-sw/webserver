#ifndef __THREAD_POOL_HPP
#define __THREAD_POOL_HPP

#include "SyncQueue.hpp"

#include <thread>
#include <vector>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t threadSize, size_t queueSize);
    ~ThreadPool();

    template <typename F, typename... Args>
    void submit(F&& f, Args&& ...args);

    void stop();

private:
    void func();
    void stopThread();

    std::vector<std::shared_ptr<std::thread>> workers_;
    SyncQueue<std::function<void()>> queue_;
    size_t maxSize_;
    bool stop_;
    std::once_flag flag_;
};

template <typename F, typename... Args>
void ThreadPool::submit(F&& f, Args&& ...args) {
    auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    queue_.put(task);
}

#endif //__THREAD_POOL_HPP
