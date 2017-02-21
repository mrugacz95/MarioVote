//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_SERVERSOCKET_H
#define UMARIO_SERVERSOCKET_H

#include "Socket.h"

class ServerSocket : public Socket {

public:
    ServerSocket(const char * ipv4Address = nullptr, const uint16_t port = 0);

    void bindAddressToSocket();
    void listen(int queueSize);
    int accept();

};


#endif //UMARIO_SERVERSOCKET_H
