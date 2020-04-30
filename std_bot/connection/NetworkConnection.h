#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include "Connection.h"
#include "socket_include.h"
#include "time_include.h"
#include "Logger.h"

class NetworkConnection : public Connection {
public:
    NetworkConnection();
    NetworkConnection(const NetworkConnection& other);

    virtual NetworkConnection& operator=(const NetworkConnection& other); 
    virtual ~NetworkConnection();

    // virtual sp<Message> readMessage();

    virtual void disconnect() { close(socket); };

    void setSocket(int sock) { socket = sock; };
    void setConnect() { connected = true; }

    virtual bool isThereMessage();

// private:
public:
    int socket;
    string connectedAddress;

    // Max number of bytes to read in one take
    // If the message is longer, will wait before reading the rest
    static const int maxReadLength = 0; 

    //    v  : wtf is that even supposed to mean
    // (Read?) File descriptor set, used with select, the function to tell if
    // there is data to read in the socket 
    fd_set rfd;

    // Timeout for select, the function to tell if there is data to read
    // in the socket
    const timeval selectTimeout = {0, 0};

    bool isPending = false;
    sp<MessageDataBuffer> pendingData;
    int pendingDataFinalLength;
    timeval timeLastPendingUpdate;
    timeval maxTimeBetweenPendingUpdates;


    virtual bool writeData(sp<MessageDataBuffer> data);
    virtual sp<MessageDataBuffer> readData(int length);
};

#endif