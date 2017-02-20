#pragma once

#ifndef UMARIO_SERVER_H
#define UMARIO_SERVER_H

//STD
#include <exception>
#include <iostream>

//NETWORK
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server {
    int socketDescriptor;
    sockaddr_in address;

    int createSocket();
    sockaddr_in createAddress();
    void setSocketAddress();

public:
    Server();
};


#endif //UMARIO_SERVER_H
