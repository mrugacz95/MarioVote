//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_CLIENT_H
#define UMARIO_CLIENT_H

#include "ClientSocket.h"

class Client {
    ClientSocket socket;

public:
    Client(const char *ipv4Address, const uint16_t port);
    ~Client();

    void connect();
};


#endif //UMARIO_CLIENT_H
