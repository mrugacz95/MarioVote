//
// Created by nuurek on 20.02.17.
//

#include "Server.h"


Server::Server() {
    serverDescriptor = createSocket();
    setSocketAddress();

    std::cout << "Created server\n";
}

Server::~Server() {
    for (const auto& clientDescriptor : clientsDescriptors) {
        close(clientDescriptor);
    }
    close(serverDescriptor);

    std::cout << "Closed server\n";
}

void Server::listen() {
    ::listen(serverDescriptor, QUEUE_SIZE);
    std::cout << "Listening on: " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << "\n";

    while(true) {
        int clientDescriptor = acceptClientConnection();
        clientsDescriptors.insert(clientDescriptor);
    }
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
    std::string interfaceAddress = getInterfaceAddress();
    address.sin_addr.s_addr = inet_addr(interfaceAddress.c_str());
    address.sin_port = htons(GLOBAL_CONFIG.getServerPort());

    return address;
}

void Server::setSocketAddress() {
    if (serverDescriptor == 0) {
        throw std::logic_error("Cannot set address of non existing socket.");
    }

    address = createAddress();

    const int optionValue = 1;
    setsockopt(serverDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*) &optionValue, sizeof(optionValue));

    int bindResult = bind(serverDescriptor, (sockaddr*) &address, sizeof(address));

    if (bindResult == -1) {
        throw std::runtime_error("Error while binding address to server socket.");
    }
}

std::string Server::getInterfaceAddress() {
    struct ifaddrs *interfacesAddresses, *interfaceAddress;
    char host[NI_MAXHOST];
    int family;

    int result = getifaddrs(&interfacesAddresses);
    if (result == -1) {
        throw std::runtime_error("Cannot get info about addresses.");
    }

    for (interfaceAddress = interfacesAddresses;
         interfaceAddress != nullptr;
         interfaceAddress = interfaceAddress->ifa_next) {

        family = interfaceAddress->ifa_addr->sa_family;

        if (family == AF_INET) {
            result = getnameinfo(interfaceAddress->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL,
                                 0, NI_NUMERICHOST);
            if (result == -1) {
                throw std::runtime_error("Cannot get exact info about address.");
            }

            if (std::string(host).find("127.0.0.1") != std::string::npos) {
                continue;
            }

            return std::string(host);
        }
    }
}

int Server::acceptClientConnection() {
    sockaddr_in clientAddress{0};
    socklen_t clientAddressSize = sizeof(clientAddress);

    auto clientDescriptor = accept(serverDescriptor, (sockaddr*) &clientAddress, &clientAddressSize);
    if(clientDescriptor == -1) {
        throw std::runtime_error("Error while accepting client socket.");
    }

    std::cout << "New connection from: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port);
    std::cout << "(fd: " << clientDescriptor << ")\n";

    return clientDescriptor;
}
