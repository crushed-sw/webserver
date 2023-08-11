#include "../include/ServerApplication.hpp"
#include "../include/MySqlConn.hpp"
#include "../include/HttpServer.hpp"

#include "controller/include/login.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

int main(int argc, char** argv) {
    ServerApplication::init(argc, argv);

    HttpServer server;
    server.setCallBack("/login", Request::GET, std::bind(&LoginController::login, new LoginController(), _1, _2));
    server.setCallBack("/register", Request::POST, std::bind(&LoginController::registe, new LoginController(), _1, _2));
    server.start();
}
