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

    // Should look like : readWrap -> unwrapData -> readMessageData -> msg->desirialize(msgData)
    virtual sp<Message> readMessage() { return nullptr; };
    virtual vector<sp<Message>> readMessages(int n) { return vector<sp<Message>>(); };
    virtual vector<sp<Message>> readAllMessages() { return vector<sp<Message>>(); };

    virtual bool sendMessage(sp<Message> msg) { return false; };
    virtual bool isThereMessage() = 0; 

    virtual bool isConnected() { return connected; };

// protected:
public:
    bool connected = false;
    

    virtual bool writeData(sp<MessageDataBuffer> data) { return false; };
    virtual sp<MessageDataBuffer> readData(int length) { return nullptr; };

    // Write header to identify the type and other descritors of the message (length for example)
    virtual void wrapData(sp<MessageDataBuffer> data, sp<Message> msg) {};
    // Read header to identify the type and other descritors of the message (length for example)
    // This step should not fully load the message, but rather readMessage()
    // This step should be a preload, where you gather information on how much data you need to read
    // to load the message and/or how to interpret it
    // TODO : more suitable name
    virtual sp<Message> unwrapData(sp<MessageDataBuffer> data) { return nullptr; };
};

#endif