#ifndef DUMMY_SERVER_H
#define DUMMY_SERVER_H

#include "Socket.h"
#include "Dynamic_array.h"
#include "Str.h"

class DummyClient;

class DummyServer : public Socket {
private:
    Dynamic_array<DummyClient*> connectedClients;

public:
    DummyServer();
    ~DummyServer();

    void acceptClient(DummyClient* client);
    void broadcast(const Str& message);
    const Dynamic_array<DummyClient*>& getConnectedClients() const;
};

#endif // DUMMY_SERVER_H
