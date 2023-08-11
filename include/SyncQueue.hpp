#ifndef __SYNC_QUEUE_HPP
#define __SYNC_QUEUE_HPP

#include <mutex>
#include <list>
#include <condition_variable>

template <typename T>
class SyncQueue {
public:
    SyncQueue(int num = 4);

    void setNum(int num);

    void put(const T& t);
    void put(T&& t);
    void take(std::list<T>& list);
    void take(T& t);

    void stop();
    bool empty();
    bool full();
    size_t size();
    int count();

private:
    template <typename F>
    void add(F&& f);

    std::list<T> list_;
    std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    int maxSize_;
    bool stop_;
};

template <typename T>
SyncQueue<T>::SyncQueue(int num) : maxSize_(num), stop_(false) {}

template <typename T>
void SyncQueue<T>::setNum(int num) {
    maxSize_ = num;
}

template <typename T>
void SyncQueue<T>::put(const T& t) {
    add(t);
}

template <typename T>
void SyncQueue<T>::put(T&& t) {
    add(std::forward<T>(t));
}

template <typename T>
void SyncQueue<T>::take(std::list<T>& list) {
    std::unique_lock<std::mutex> locker(mutex_);
    notEmpty_.wait(locker, [this](){ return stop_ || !list_.empty(); });
    if(stop_)
        return;

    list = std::move(list);
    notFull_.notify_one();
}

template <typename T>
void SyncQueue<T>::take(T& t) {
    std::unique_lock<std::mutex> locker(mutex_);
    notEmpty_.wait(locker, [this](){ return stop_ || !list_.empty(); });
    if(stop_)
        return;

    t = std::move(list_.front());
    list_.pop_front();
    notFull_.notify_one();
}

template <typename T>
void SyncQueue<T>::stop() {
    {
        std::lock_guard<std::mutex> locker(mutex_);
        stop_ = true;
    }
    notFull_.notify_all();
    notEmpty_.notify_all();
}

template <typename T>
bool SyncQueue<T>::empty() {
    std::lock_guard<std::mutex> locker(mutex_);
    return list_.empty();
}

template <typename T>
bool SyncQueue<T>::full() {
    std::lock_guard<std::mutex> locker(mutex_);
    return list_.size() >= maxSize_;
}

template <typename T>
size_t SyncQueue<T>::size() {
    std::lock_guard<std::mutex> locker(mutex_);
    return list_.size();
}

template <typename T>
int SyncQueue<T>::count() {
    return list_.size();
}

template <typename T>
template <typename F>
void SyncQueue<T>::add(F&& f) {
    std::unique_lock<std::mutex> locker(mutex_);
    notFull_.wait(locker, [this](){ return stop_ || list_.size() < maxSize_; });
    if(stop_)
        return;

    list_.push_back(std::forward<F>(f));
    notEmpty_.notify_one();
}

#endif //__SYNC_QUEUE_HPP
