//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_CONFIG_H
#define UMARIO_CONFIG_H

#include <string>
#include <fstream>
#include "lib/json.hpp"

using json = nlohmann::json;

class Config {
    json configJSON;
    u_int16_t serverPort;

public:
    Config(){}

    Config(std::string filename) {
        std::ifstream file(filename);
        file >> configJSON;

        serverPort = configJSON["serverPort"];
    }

    uint16_t getServerPort() {
        return serverPort;
    }
};

extern Config GLOBAL_CONFIG;

#endif //UMARIO_CONFIG_H
