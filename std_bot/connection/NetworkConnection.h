#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include "Connection.h"
#include "socket_include.h"
#include "Logger.h"

class NetworkConnection : public Connection {
public:
    NetworkConnection();
    NetworkConnection(const NetworkConnection& other);

    virtual NetworkConnection& operator=(const NetworkConnection& other); 
    virtual ~NetworkConnection();

    virtual void disconnect() { close(socket); connected = false; };

    void setSocket(int sock) { socket = sock; };
    void setConnect() { connected = true; }

    virtual bool isThereMessage();

    virtual void temp() override { cout << "Network connection" << endl; };
// private:
public:
    int socket;
    string connectedAddress;

    // TODO
    void emptySocketBuffer() {};

    //    v  : wtf is that even supposed to mean
    // (Read?) File descriptor set, used with select, the function to tell if
    // there is data to read in the socket 
    fd_set rfd;

    // Timeout for select, the function to tell if there is data to read
    // in the socket
    const timeval selectTimeout = {0, 0};

    virtual bool writeData(sp<MessageDataBuffer> data);
    virtual sp<MessageDataBuffer> readData(int length);
};

#endif