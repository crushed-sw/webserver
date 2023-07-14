#ifndef __LOG_HPP
#define __LOG_HPP

#include <string>
#include <fstream>

#include "SyncQueue.hpp"
#include "Timer.hpp"

class Log {
public:
    enum level {INFO, DEBUG, WARN, ERROR};
    struct End {};

    Log(level lev);
    ~Log();

    Log& operator<<(const std::string& t);
    Log& operator<<(int i);
    Log& operator<<(double d);
    Log& operator<<(long int l);
    Log& operator<<(End end);

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
extern Log::End End;

#endif //__LOG_HPP
