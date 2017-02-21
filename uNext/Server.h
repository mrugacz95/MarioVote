#pragma once

#ifndef UMARIO_SERVER_H
#define UMARIO_SERVER_H

//STD
#include <exception>
#include <iostream>
#include <string>

//NETWORK
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>


class Server {
    int serverDescriptor;
    sockaddr_in address;

    int createSocket();
    sockaddr_in createAddress();
    void setSocketAddress();
    std::string getInterfaceAddress();

    int acceptClientConnection();

public:
    static const int QUEUE_SIZE = 10;

    Server();
    ~Server();

    void listen();
};


#endif //UMARIO_SERVER_H
