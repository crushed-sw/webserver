#include "../include/Timer.hpp"

#include <ctime>
#include <cstring>
#include <iomanip>

Timer::Timer() {
    update();
}

timeval Timer::getNow() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return std::move(tv);
}

std::chrono::milliseconds Timer::getNowChrono() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
}

std::tm Timer::getNowTm(long int& millisecond) {
    timeval time = getNow();
    millisecond = time.tv_usec;
    std::tm local = {};
    localtime_r(&time.tv_sec, &local);
    return std::move(local);
}

std::string Timer::getNowString() {
    update();
    char buf[30];
    sprintf(buf, "%04ld_%02ld_%02ld %02ld:%02ld:%02ld.%06ld", year_, month_, day_, hour_, miniute_, second_, millisecond_);
    return std::move(std::string(buf, strlen(buf)));
}

std::string Timer::getNowString(std::string str) {
    long int millisecond = 0;
    std::tm time = getNowTm(millisecond);
    std::stringstream ss;
    ss << std::put_time(&time, str.c_str());
    buffer_ << ss.str() << "." << millisecond;
    return buffer_.getAll();
}

void Timer::update() {
    std::tm time = getNowTm(millisecond_);
    year_ = time.tm_year + 1900;
    month_ = time.tm_mon + 1;
    day_ = time.tm_mday;
    hour_ = time.tm_hour;
    miniute_ = time.tm_min;
    second_ = time.tm_sec;
}

long int Timer::getYear() {
    return year_;
}

long int Timer::getMonth() {
    return month_;
}

long int Timer::getDay() {
    return day_;
}

long int Timer::getHour() {
    return hour_;
}

long int Timer::getMinute() {
    return miniute_;
}

long int Timer::getSecond() {
    return second_;
}

long int Timer::getMilliSecond() {
    return millisecond_;
}
