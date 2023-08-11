#include "../include/login.hpp"

std::string LoginController::login(Request& request, Response& response) {
    response.setHeader("Content-Type", "application/json");

    auto parameters = request.getParameter();
    std::string username = parameters["username"];
    std::string password = parameters["password"];

    if(username == "" || password == "") {
        Message<std::string> message(Codes::paraError, "参数错误");
        return message.parse();
    }

    User user = service_.query(username);

    if(user.getUsername() == "" || user.getPassword() != password) {
        Message<std::string> message(Codes::paraError, "用户名或密码错误");
        return message.parse();
    }

    Message<std::string> message(Codes::success, "登陆成功");
    return message.parse();
}

std::string LoginController::registe(Request& request, Response& response) {
    response.setHeader("Content-Type", "application/json");

    auto parameters = request.getParameter();

    std::string username = parameters["username"];
    std::string password = parameters["password"];

    if(username == "" || password == "") {
        Message<std::string> message(Codes::paraError, "参数错误");
        return message.parse();
    }

    User user = service_.query(username);
    if(user.getUsername() != "") {
        Message<std::string> message(Codes::paraError, "用户名已存在");
        return message.parse();
    }

    user.setUsername(username);
    user.setPassword(password);
    service_.insert(user);

    Message<std::string> message(Codes::success, "注册成功");
    return message.parse();
}
