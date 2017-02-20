#pragma once

#ifndef UMARIO_SERVER_H
#define UMARIO_SERVER_H

//STD
#include <exception>
#include <iostream>
#include <string>

//NETWORK
#define _GNU_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>


class Server {
    int socketDescriptor;
    sockaddr_in address;

    int createSocket();
    sockaddr_in createAddress();
    void setSocketAddress();
    std::string getInterfaceAddress();

public:
    Server();
};


#endif //UMARIO_SERVER_H
