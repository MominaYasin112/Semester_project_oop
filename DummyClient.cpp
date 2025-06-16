#include "DummyClient.h"
#include "DummyServer.h"
#include "AppException.h"
#include <iostream>

DummyClient::DummyClient(const Str& name) : connectedServer(nullptr), clientName(name) {}

DummyClient::~DummyClient() {}

void DummyClient::connectToServer(DummyServer* server) {
    if (!server) {
        throw NetworkException(Str("Cannot connect: Server is null."));
    }

    connectedServer = server;
    open(); // Simulate socket connection

    if (!isConnected()) {
        throw NetworkException(Str("Socket connection failed."));
    }

    server->acceptClient(this);
}
void DummyClient::sendData(const Str& message) {
    if (!connectedServer) {
        throw NetworkException(Str("No server connected."));
    }
    if (!isConnected()) {
        throw NetworkException(Str("Cannot send message: Socket not connected."));
    }

    Str fullMessage = clientName.concat(Str(": ")).concat(message);
    connectedServer->broadcast(fullMessage);
}


void DummyClient::receiveData(const Str& message) {
    std::cout << "[Message Received] " << message << std::endl;
}

Str DummyClient::getName() const {
    return clientName;
}
