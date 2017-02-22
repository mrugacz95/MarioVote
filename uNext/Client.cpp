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

std::vector<unsigned char> Client::receiveFromServer() {
    std::vector<unsigned char> response;
    response.resize(6);

    int result = read(socket.getDescriptor(), &response[0], response.size());
    if (result == -1) {
        throw std::runtime_error("Error while receiving data from server.");
    }

    return std::move(response);
}
