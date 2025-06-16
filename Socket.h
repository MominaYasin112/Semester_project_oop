#ifndef SOCKET_H
#define SOCKET_H

class Socket {
protected:
    int socketId;
    int port;
    static int nextSocketId;

public:
    Socket();
    virtual ~Socket();

    virtual void open();
    virtual void close();
    virtual bool isConnected() const;
};

#endif // SOCKET_H
