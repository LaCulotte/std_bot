#ifndef CONNECTION_H
#define CONNECTION_H

#include "std_include.h"
#include "ptr_include.h"
#include "Message.h"

class Connection{
public:
    Connection() {};
    Connection(const Connection& other) {};

    virtual Connection& operator=(const Connection& other) = default;
    virtual ~Connection() = default;

    virtual sp<Message> readMessage();
    virtual vector<sp<Message>> readMessages(int n);
    virtual vector<sp<Message>> readAllMessages();

    virtual bool send(sp<Message> msg);
    virtual bool isThereMessage(); 

protected:
    bool isConnected = false;
    

    virtual bool writeData(sp<MessageDataBuffer> data);
    virtual sp<MessageDataBuffer> readData();
    virtual void wrapData(sp<MessageDataBuffer> data, sp<Message> msg);
};

#endif