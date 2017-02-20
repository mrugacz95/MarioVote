//
// Created by nuurek on 20.02.17.
//

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
    std::string interfaceAddress = getInterfaceAddress();
    address.sin_addr.s_addr = inet_addr(interfaceAddress.c_str());

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
