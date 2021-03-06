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
#include "lib/json.hpp"

using JSON = nlohmann::json;

enum class ClientStatus {
    NOT_SYNCHRONIZED, SYNCHRONIZED
};

class Server {
    ServerSocket socket;
    std::unordered_set<int> clientsDescriptors;
    std::map<int, ClientStatus> clientsStatuses;

    bool started = false;
    void listen();

    bool sendMessage(int descriptor, JSON json);
    bool send(int descriptor, void *buffer, size_t size);

public:
    static const int QUEUE_SIZE = 10;

    Server();
    ~Server();

    void start();
    void stop();
    bool isStarted();
    void sendToClientsWithStatus(ClientStatus status, JSON json);
};


#endif //UMARIO_SERVER_H
