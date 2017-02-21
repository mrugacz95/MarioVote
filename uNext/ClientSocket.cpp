//
// Created by nuurek on 21.02.17.
//

#include "ClientSocket.h"

ClientSocket::ClientSocket(const char *ipv4Address, const uint16_t port) : Socket(ipv4Address, port) {
    std::cout << "Created client socket\n";
}

void ClientSocket::connect() {
    int result = ::connect(descriptor, (sockaddr*) &address, sizeof(address));

    if(result == -1) {
        throw std::runtime_error("Error while connecting to server.");
    }
}
