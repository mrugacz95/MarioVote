//
// Created by nuurek on 20.02.17.
//

#include "Server.h"


Server::Server() :
    socket(nullptr, GLOBAL_CONFIG.getServerPort())
{
    socket.bindAddressToSocket();
    std::cout << "Created server\n";
}

Server::~Server() {
    for (auto& clientDescriptor : clientsDescriptors) {
        close(clientDescriptor);
    }

    std::cout << "Destroyed server\n";
}

void Server::listen() {
    socket.listen(QUEUE_SIZE);
    std::cout << "Listening on: " << inet_ntoa(socket.getAddress()) << ":" << ntohs(socket.getPort()) << "\n";

    while(isStarted) {
        int clientDescriptor;
        try {
            clientDescriptor = socket.accept();
        } catch (std::runtime_error) {
            break;
        }
        clientsDescriptors.insert(clientDescriptor);
    }
}

void Server::start() {
    isStarted = true;

    std::thread clientListenThread(&Server::listen, this);
    clientListenThread.detach();
}

void Server::stop() {
    isStarted = false;
}
