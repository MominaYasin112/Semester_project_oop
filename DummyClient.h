// DummyClient.h
#ifndef DUMMY_CLIENT_H
#define DUMMY_CLIENT_H

#include "Socket.h"
#include "Str.h"

class DummyServer;

class DummyClient : public Socket {
private:
    DummyServer* connectedServer;
    Str clientName;

public:
    DummyClient(const Str& name);
    ~DummyClient();

    void connectToServer(DummyServer* server);
    void sendData(const Str& message);
    void receiveData(const Str& message);
    Str getName() const;
};

#endif // DUMMY_CLIENT_H
