//
// Created by nuurek on 21.02.17.
//

#include "ServerSocket.h"


ServerSocket::ServerSocket(const char *ipv4Address, const uint16_t port) : Socket(ipv4Address, port) {
    std::cout << "Created server socket\n";
}

void ServerSocket::bindAddressToSocket() {
    int result = bind(descriptor, (sockaddr*) &address, sizeof(address));

    if (result == -1) {
        throw std::runtime_error("Error while binding address to server socket.");
    }
}

void ServerSocket::listen(int queueSize) {
    int result = ::listen(descriptor, queueSize);

    if (result == -1) {
        throw std::runtime_error("Error while trying to listen on server socket.");
    }
}

int ServerSocket::accept() {
    sockaddr_in clientAddress{0};
    socklen_t clientAddressSize = sizeof(clientAddress);

    auto clientDescriptor = ::accept(descriptor, (sockaddr*) &clientAddress, &clientAddressSize);
    if(clientDescriptor == -1) {
        throw std::runtime_error("Error while accepting client socket.");
    }

    std::cout << "New connection from: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port);
    std::cout << "(fd: " << clientDescriptor << ")\n";

    return clientDescriptor;
}
