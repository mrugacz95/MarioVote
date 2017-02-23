//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_SOCKET_H
#define UMARIO_SOCKET_H

//STD
#include <exception>

//NETWORK
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include "Config.h"


class Socket {
protected:
    int descriptor;
    sockaddr_in address;

    int createSocket() const;
    sockaddr_in createAddress(const char * ipv4Address = nullptr, const uint16_t port = 0) const;
    void setSocketAddress();
    void setSocketOption(const int optionLevel, const int optionName, const int value);
    std::string getInterfaceAddress() const;

public:
    Socket(const char * ipv4Address = nullptr, const uint16_t port = 0);
    ~Socket();

    int getDescriptor() const;
    in_addr getAddress() const;
    in_port_t getPort() const;
};


#endif //UMARIO_SOCKET_H
