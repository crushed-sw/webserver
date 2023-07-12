#ifndef __LOG_HPP
#define __LOG_HPP

#include <string>
#include <fstream>

#include "SyncQueue.hpp"
#include "Timer.hpp"

enum level {
    INFO,
    DEBUG,
    WARN,
    ERROR
};

class Log {
public:
    Log(level lev);
    ~Log();

    Log& operator<<(const std::string& t);
    Log& operator<<(int i);
    Log& operator<<(double d);
    Log& operator<<(long int l);

    std::string levelToString(const level& lev);
    void update();

private:
    void changeLog();

    std::string sourceDir_;
    std::string logFile_;
    long int last_;
    Timer timer_;
    level lev_;
    int fd_;

    static SyncQueue<std::string> queue_;
    std::mutex mutex_;

    Buffer buffer_;
};

extern Log Info;
extern Log Debug;
extern Log Warn;
extern Log Error;

#endif //__LOG_HPP
