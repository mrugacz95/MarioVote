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

JSON Client::receiveFromServer() {
    std::string response;
    std::vector<char> buffer(4096);

    auto bytes = read(socket.getDescriptor(), &buffer[0], buffer.size());
    if (bytes == -1) {
        throw std::runtime_error("Error while receiving data from server.");
    }

    response.append(buffer.begin(), buffer.end());

    JSON json = JSON::parse(response);

    return json;
}
