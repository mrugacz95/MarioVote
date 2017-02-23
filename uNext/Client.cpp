//
// Created by nuurek on 21.02.17.
//

#include "Client.h"

Client::Client(const char *ipv4Address, uint16_t port) :
    socket(ipv4Address, port)
{
    std::cout << "Created client\n";
}

Client::~Client() {
    std::cout << "Closed client\n";
}

void Client::connect() {
    socket.connect();
}

JSON Client::receiveInput() {
    std::string response;
    std::vector<char> buffer(4096);

    auto bytes = read(socket.getDescriptor(), &buffer[0], buffer.size());
    if (bytes == -1) {
        throw std::runtime_error("Error while receiving data from server.");
    }

    response.append(buffer.begin(), buffer.end());
    try {
        JSON json = JSON::parse(response);

        isServerPaused = json["isPaused"];

        return json;
    }
    catch(std::invalid_argument){
        std::cout<<"parse error\n";
    }
    return NULL;

}

JSON Client::synchronizeMap() {
    return JSON();
}

bool Client::isGamePaused() {
    return isServerPaused;
}
