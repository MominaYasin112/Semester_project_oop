#include "Socket.h"
#include"AppException.h"
int Socket::nextSocketId = 1;

Socket::Socket() : socketId(-1), port(-1) {}

Socket::~Socket() {}

void Socket::open() {
    if (isConnected()) {
        throw NetworkException(Str("Socket is already open."));
    }
    socketId = nextSocketId++;
    port = 8080;
}

void Socket::close() {
    if (!isConnected()) {
        throw NetworkException(Str("Socket is not connected."));
    }
    socketId = -1;
    port = -1;
}


bool Socket::isConnected() const {
    return socketId != -1;
}
