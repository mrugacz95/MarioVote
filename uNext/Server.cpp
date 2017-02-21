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
    std::cout << "Closed server\n";
}

void Server::listen() {
    socket.listen(QUEUE_SIZE);
    std::cout << "Listening on: " << inet_ntoa(socket.getAddress()) << ":" << ntohs(socket.getPort()) << "\n";

    while(true) {
        int clientDescriptor = socket.accept();
        clientsDescriptors.insert(clientDescriptor);
    }
}