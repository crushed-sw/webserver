#include "../include/Log.hpp"

#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

Log Info(Log::INFO);
Log Debug(Log::DEBUG);
Log Warn(Log::WARN);
Log Error(Log::ERROR);
Log::End End;

SyncQueue<std::string> Log::queue_(500);

Log::Log(level lev) {
    lev_ = lev;
    update();
}

Log::~Log() {
    std::unique_lock<std::mutex> locker(mutex_);
    queue_.put(buffer_.getAll());
    std::list<std::string> list;
    queue_.take(list);
    for(std::string str : list) {
        write(fd_, str.c_str(), str.length());
    }
}

std::string Log::levelToString(const level& lev) {
    if(lev == INFO) {
        return "[info] ";
    } else if(lev == DEBUG) {
        return "[debug]";
    } else if(lev == WARN) {
        return "[warn] ";
    } else if(lev == ERROR) {
        return "[error]";
    }
    return "";
}

Log& Log::operator<<(const std::string& str) {
    update();
    {
        std::unique_lock<std::mutex> locker(mutex_);
        buffer_ << timer_.getNowString() << " " << levelToString(lev_) << " : " << str << "\n";
        queue_.put(buffer_.getLine());
    }

    {
        std::unique_lock<std::mutex> locker(mutex_);
        std::string str;
        queue_.take(str);
        str += "\n";
        write(fd_, str.c_str(), str.length());
    }

    return *this;
}

Log& Log::operator<<(int i) {
    *this << std::to_string(i);
    return *this;
}

Log& Log::operator<<(double d) {
    *this << std::to_string(d);
    return *this;
}

Log& Log::operator<<(long int l) {
    *this << std::to_string(l);
    return *this;
}

void Log::update() {
    long int millisecond;
    timer_.update();
    
    if(sourceDir_.empty()) {
        char buf[1024] = {};
        getcwd(buf, sizeof(buf));
        sourceDir_ = std::string(buf, strlen(buf)) + "/log";
        
        mkdir(sourceDir_.c_str(), 0775);
    }

    if(last_ != timer_.getDay()) {
        char buf[18];
        close(fd_);
        sprintf(buf, "/%04ld_%02ld_%02ld.log", timer_.getYear(), timer_.getMonth(), timer_.getDay());
        logFile_ = sourceDir_ + std::move(std::string(buf, strlen(buf)));
        last_ = timer_.getDay();
        fd_ = open(logFile_.c_str(), O_CREAT | O_RDWR | O_APPEND, 0664);
    }
}
