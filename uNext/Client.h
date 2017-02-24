//
// Created by nuurek on 21.02.17.
//

#ifndef UMARIO_CLIENT_H
#define UMARIO_CLIENT_H

#include <vector>

#include "ClientSocket.h"
#include "lib/json.hpp"

using JSON = nlohmann::json;


class Client {
    ClientSocket socket;

    bool isServerPaused = true;

    unsigned long receiveSizeOfNextMessage();
    void receive(void *buffer, size_t size);
    bool sendMessage(int descriptor, JSON message);
public:
    Client(const char *ipv4Address, const uint16_t port);
    ~Client();

    void connect();
    JSON receiveMessage();
    bool isGamePaused();

    bool isSynchronized = false;

    void sendVoteToServer(const JSON &vote);
};


#endif //UMARIO_CLIENT_H
