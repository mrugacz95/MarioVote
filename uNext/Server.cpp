//
// Created by nuurek on 20.02.17.
//

#include "Server.h"


Server::Server() :
    socket(GLOBAL_CONFIG.getServerPort())
{
    std::cout << "Created server\n";
}

Server::~Server() {
    for (const auto& clientDescriptor : clientsDescriptors) {
        close(clientDescriptor);
    }
    close(socket.getDescriptor());

    std::cout << "Closed server\n";
}

void Server::listen() {
    ::listen(socket.getDescriptor(), QUEUE_SIZE);
    std::cout << "Listening on: " << inet_ntoa(socket.getAddress()) << ":" << ntohs(socket.getPort()) << "\n";

    while(true) {
        int clientDescriptor = acceptClientConnection();
        clientsDescriptors.insert(clientDescriptor);
    }
}

int Server::acceptClientConnection() {
    sockaddr_in clientAddress{0};
    socklen_t clientAddressSize = sizeof(clientAddress);

    auto clientDescriptor = accept(socket.getDescriptor(), (sockaddr*) &clientAddress, &clientAddressSize);
    if(clientDescriptor == -1) {
        throw std::runtime_error("Error while accepting client socket.");
    }

    std::cout << "New connection from: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port);
    std::cout << "(fd: " << clientDescriptor << ")\n";

    return clientDescriptor;
}
