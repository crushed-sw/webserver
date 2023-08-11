#include "../include/ServerApplication.hpp"

ServerConfig ServerApplication::config_;

ServerApplication::ServerApplication() {}

ServerApplication& ServerApplication::getInstance() {
    static ServerApplication application;
    return application;
}

void showHelp() {
    std::cout << "command -i <configfile>" << std::endl;
}

void ServerApplication::init(int argc, char** argv) {
    if(argc < 3) {
        showHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string configFile;
    while((c = getopt(argc, argv, "i:")) != -1) {
        switch(c) {
            case 'i':
                configFile = optarg;
                break;
            case '?':
            case ':':
            default:
                showHelp();
                exit(EXIT_FAILURE);
                break;
        }
    }

    config_.loadConfigFile(configFile);
}

ServerConfig& ServerApplication::getConfig() {
    return config_;
}
