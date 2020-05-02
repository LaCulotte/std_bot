#ifndef CONNECTION_H
#define CONNECTION_H

#include "std_include.h"
#include "ptr_include.h"
#include "Message.h"
#include "MessageTypeHandler.h"

class Connection{
public:
    Connection() {};
    Connection(const Connection& other) {};

    virtual Connection& operator=(const Connection& other) = default;
    virtual ~Connection() = default;

    virtual void initHandler() {}; 

    // If data the message returned is invalid, discard the message. Then there is two options : 
    // 1. Empty the socket input buffer and wait for other messages to come
    // Pros : You make sure to not interpret wrong message.
    // Cons : You may lose some messages
    // 2. Ignore the invalid message and treat the following data as if it was a new message
    // Pros : You are almost sure to not lose any message (not true if the header is more than one byte)
    // Cons : You may interpret message data as a header. If this header is valid, you may interpret a wrong message
    //        that can mess with the system continuity
    // In my opinion, you should use the first solution if the messages received are few and big, and you can handle losing some messages
    //                you should use the second solution if you have to (and can) make sure that you do not lose any message

    virtual sp<Message> readMessage() { return nullptr; };
    virtual vector<sp<Message>> readMessages(int n) { return vector<sp<Message>>(); };
    virtual vector<sp<Message>> readAllMessages() { return vector<sp<Message>>(); };

    virtual bool sendMessage(sp<Message> msg) { return false; };
    virtual bool isThereMessage() { return false; }; 

    virtual void temp() { cout << "Connection" << endl; };

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

    sp<MessageTypeHandler> msgTypeHandler;

};

#endif