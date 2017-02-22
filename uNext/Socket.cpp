//
// Created by nuurek on 21.02.17.
//

#include <stdexcept>
#include "Socket.h"

Socket::Socket(const char * ipv4Address, const uint16_t port) {
    descriptor = createSocket();
    address = createAddress(ipv4Address, port);
    setSocketAddress();
}

int Socket::createSocket() const {
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor == -1) {
        throw std::runtime_error("Error while creating socket.");
    }

    return descriptor;
}

sockaddr_in Socket::createAddress(const char * ipv4Address, const uint16_t port) const {
    sockaddr_in address;

    address.sin_family = AF_INET;

    if (ipv4Address == nullptr) {
        address.sin_addr.s_addr = inet_addr(getInterfaceAddress().c_str());
    } else {
        address.sin_addr.s_addr = inet_addr(ipv4Address);
    }

    address.sin_port = htons(port);

    return address;
}

void Socket::setSocketAddress() {
    if (descriptor == 0) {
        throw std::logic_error("Cannot set address of non existing socket.");
    }

    setSocketOption(1);
}

void Socket::setSocketOption(const int optionValue) {
    int result = setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, (char*) &optionValue, sizeof(optionValue));

    if (result == -1) {
        throw std::runtime_error("Error while setting socket option.");
    }
}

std::string Socket::getInterfaceAddress() const {
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

int Socket::getDescriptor() const {
    return descriptor;
}

in_addr Socket::getAddress() const {
    return address.sin_addr;
}

in_port_t Socket::getPort() const {
    return address.sin_port;
}

Socket::~Socket() {
    close(descriptor);
}
