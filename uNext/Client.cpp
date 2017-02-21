//
// Created by nuurek on 21.02.17.
//

#include "Client.h"

Client::Client(const char *ipv4Address, uint16_t port) :
    socket(ipv4Address, port)
{
    std::cout << "Created client\n";
}

Client::~Client() {
    std::cout << "Closed client\n";
}
