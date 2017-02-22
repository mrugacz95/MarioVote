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

    while(started) {
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
    started = true;

    std::thread clientListenThread(&Server::listen, this);
    clientListenThread.detach();
}

void Server::stop() {
    started = false;
}

void Server::sendToClients(char *buffer, int count) {
    int result;
    std::unordered_set<int> badClientsDescriptors;

    for (auto& clientDescriptor : clientsDescriptors) {
        result = write(clientDescriptor, buffer, count);

        if (result != count) {
            badClientsDescriptors.insert(clientDescriptor);
        }
    }

    for (auto& clientDescriptor : badClientsDescriptors) {
        clientsDescriptors.erase(clientDescriptor);
    }
}

bool Server::isStarted() {
    return started;
}
