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
        clientsStatuses[clientDescriptor] = ClientStatus::NOT_SYNCHRONIZED;
        //clientsStatuses[clientDescriptor] = ClientStatus::SYNCHRONIZED;
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

void Server::sendToClientsWithStatus(ClientStatus status, JSON json) {
    std::unordered_set<int> badClientsDescriptors;

    for (auto& clientDescriptor : clientsDescriptors) {
        if (clientsStatuses[clientDescriptor] == status) {
            if (sendMessage(clientDescriptor, json)) {
                clientsStatuses[clientDescriptor] = ClientStatus::SYNCHRONIZED;
            } else {
                badClientsDescriptors.insert(clientDescriptor);
            }
        }
    }

    for (auto& clientDescriptor : badClientsDescriptors) {
        clientsDescriptors.erase(clientDescriptor);
        std::cout << "Removing connection (fd: " << clientDescriptor << ")\n";
    }
}

bool Server::isStarted() {
    return started;
}

bool Server::sendMessage(int descriptor, JSON message) {
    auto dumped = message.dump();
    auto size = dumped.size();

    std::cout << "Message size: " << size << "\n";

    return (send(descriptor, &size, sizeof(unsigned long)) && send(descriptor, &dumped[0], size));
}

bool Server::send(int descriptor, void *buffer, size_t size) {
    ssize_t bytesWritten;

    while (size != 0) {
        bytesWritten = write(descriptor, buffer, size);

        if (bytesWritten == -1) {
            std::cerr << "Error while sending data to client.\n";
            return false;
        }

        buffer += bytesWritten;
        size -= bytesWritten;
    }

    return true;
}
