//
// Created by nuurek on 20.02.17.
//

#include <stdexcept>
#include "Server.h"


Server::Server() {
    socketDescriptor = createSocket();
    setSocketAddress();

    std::cout << "Created server\n";
}

int Server::createSocket() {
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor == -1) {
        throw std::runtime_error("Error while creating socket.");
    }

    return descriptor;
}

sockaddr_in Server::createAddress() {
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    return address;
}

void Server::setSocketAddress() {
    if (socketDescriptor == 0) {
        throw std::logic_error("Cannot set address of non existing socket.");
    }

    address = createAddress();

    const int optionValue = 1;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, (char*) &optionValue, sizeof(optionValue));

    int bindResult = bind(socketDescriptor, (sockaddr*) &address, sizeof(address));

    if (bindResult == -1) {
        throw std::runtime_error("Error while binding address to server socket.");
    }
}