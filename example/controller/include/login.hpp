#ifndef __LOGIN_HPP
#define __LOGIN_HPP

#include "../../../include/HttpServer.hpp"

#include "../../util/include/message.hpp"
#include "../../util/include/codes.hpp"
#include "../../entity/include/user.hpp"
#include "../../service/include/userService.hpp"

#include <string>
#include <map>

class LoginController {
public:
    std::string login(Request& request, Response& response);
    std::string registe(Request& request, Response& response);

private:
    UserService service_;
};

#endif //__LOGIN_HPP
