#ifndef __TIMER_HPP
#define __TIMER_HPP

#include "Buffer.hpp"

#include <chrono>
#include <string>

#include <sys/time.h>

class Timer {
public:
    Timer();

    timeval getNow();
    std::chrono::milliseconds getNowChrono();
    std::string getNowString(std::string str);
    std::string getNowString();
    std::tm getNowTm(long int& milliseconds);

    void update();

    long int getYear();
    long int getMonth(); 
    long int getDay();
    long int getHour();
    long int getMinute();
    long int getSecond();
    long int getMilliSecond();

private:
    long int year_;
    long int month_;
    long int day_;
    long int hour_;
    long int miniute_;
    long int second_;
    long int millisecond_;

    Buffer buffer_;
};

#endif //__TIMER_HPP
