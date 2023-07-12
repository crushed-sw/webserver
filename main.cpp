#include "include/Log.hpp"

#include <thread>
#include <vector>

void logInfo() {
    for(int i = 0; i != 10000; ++i) {
        Info << "info";
    }
}

void logDebug() {
    for(int i = 0; i != 10000; ++i) {
        Debug << "debug";
    }
}

void logWarn() {
    for(int i = 0; i != 10000; ++i) {
        Warn << "warn";
    }
}

void logError() {
    for(int i = 0; i != 10000; ++i) {
        Error << "erorr";
    }
}

int main() {
    std::thread v1(logInfo);
    std::thread v2(logDebug);
    std::thread v3(logWarn);
    std::thread v4(logError);

    v1.join();
    v2.join();
    v3.join();
    v4.join();
}
