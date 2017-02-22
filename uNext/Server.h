#pragma once

#ifndef UMARIO_SERVER_H
#define UMARIO_SERVER_H

//STD
#include <exception>
#include <iostream>
#include <string>
#include <unordered_set>
#include <thread>

//NETWORK
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include "ServerSocket.h"


class Server {
    ServerSocket socket;
    std::unordered_set<int> clientsDescriptors;

    bool started = false;
    void listen();

public:
    static const int QUEUE_SIZE = 10;

    Server();
    ~Server();

    void start();
    void stop();
    bool isStarted();
    void sendToClients(char * buffer, int count);
};


#endif //UMARIO_SERVER_H
