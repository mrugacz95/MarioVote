//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_CLIENT_H
#define UMARIO_CLIENT_H

#include "Socket.h"

class Client {
    Socket socket;

public:
    Client(const char *ipv4Address, uint16_t port);
    ~Client();
};


#endif //UMARIO_CLIENT_H
