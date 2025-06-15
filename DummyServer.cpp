#include "DummyServer.h"
#include "DummyClient.h"
#include"AppException.h"
#include <iostream>

DummyServer::DummyServer() {}

DummyServer::~DummyServer() {}

void DummyServer::acceptClient(DummyClient* client) {
    connectedClients.push(client);
    std::cout << "New client connected: " << client->getName() << std::endl;
}

void DummyServer::broadcast(const Str& message) {
    if (connectedClients.size() == 0) {
        throw NetworkException(Str("Broadcast failed: No connected clients."));
    }

    for (int i = 0; i < connectedClients.size(); ++i) {
        connectedClients[i]->receiveData(message);
    }
}


const Dynamic_array<DummyClient*>& DummyServer::getConnectedClients() const {
    return connectedClients;
}
