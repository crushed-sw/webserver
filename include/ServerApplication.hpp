#ifndef __SERVER_APPLICATION_HPP
#define __SERVER_APPLICATION_HPP

#include "ServerConfig.hpp"

#include <unistd.h>

#include <iostream>
#include <string>

class ServerApplication {
public:
    static ServerApplication& getInstance();
    static void init(int argc, char** argv);
    static ServerConfig& getConfig();
    
private:
    ServerApplication();

    ServerApplication(const ServerApplication&) = delete;
    ServerApplication(ServerApplication&&) = delete;
    ServerApplication operator=(const ServerApplication&) = delete;
    ServerApplication operator=(ServerApplication&&) = delete;

    static ServerConfig config_;
};

#endif //__SERVER_APPLICATION_HPP
