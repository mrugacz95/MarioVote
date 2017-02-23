//
// Created by nuurek on 21.02.17.
//

#include "Client.h"

Client::Client(const char *ipv4Address, uint16_t port) :
    socket(ipv4Address, port)
{
    std::cout << "Created client\n";
}

Client::~Client() {
    std::cout << "Closed client\n";
}

void Client::connect() {
    socket.connect();
}

JSON Client::receiveMessage() {
    std::string response;

    const unsigned long sizeOfMessage = receiveSizeOfNextMessage();

    std::vector<char> buffer(sizeOfMessage);
    receive(&buffer[0], sizeOfMessage);
    response.append(buffer.begin(), buffer.end());

    JSON json;
    try {
        json = JSON::parse(response);
    } catch(std::invalid_argument) {
        std::cout << "Parse error!\nDumped: " << response << "\n";
    }

    if (json.find("isPaused") != json.end()) {
        isServerPaused = json["isPaused"];
    }

    return json;
}

bool Client::isGamePaused() {
    return isServerPaused;
}

unsigned long Client::receiveSizeOfNextMessage() {
    unsigned long size = 0;

    receive(&size, sizeof(unsigned long));

    return size;
}

void Client::receive(void *buffer, size_t size) {
    ssize_t bytesRead = 0;

    while(size != 0) {
        bytesRead += read(socket.getDescriptor(), buffer, size);

        if (bytesRead == -1) {
            throw std::runtime_error("Error while receiving data from server.");
        }

        buffer += bytesRead;
        size -= bytesRead;
    }
}
