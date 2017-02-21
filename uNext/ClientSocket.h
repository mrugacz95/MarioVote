//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_CLIENTSOCKET_H
#define UMARIO_CLIENTSOCKET_H

#include "Socket.h"

class ClientSocket : public Socket {

public:
    ClientSocket(const char * ipv4Address = nullptr, const uint16_t port = 0);

    void connect();
};


#endif //UMARIO_CLIENTSOCKET_H
