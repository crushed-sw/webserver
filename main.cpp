#include "include/MySql.hpp"

#include <iostream>

int main() {
    MySql mysql("localhost", "root", "zk200111", "webserver");

    auto vec = mysql.prepare("select * from ?").bindParameter({std::string("user")}).getResult();
    for(auto v : vec) {
        for(auto s : v) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
}
