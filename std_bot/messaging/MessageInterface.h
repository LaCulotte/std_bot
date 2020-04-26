#ifndef MESSAGEINTERFACE_H
#define MESSAGEINTERFACE_H

#include "Message.h"
#include "thread_include.h"
#include "Logger.h"

class MessageInterface{
public:
    MessageInterface();
    MessageInterface(const MessageInterface &other);

    ~MessageInterface();

    MessageInterface& operator=(const MessageInterface &other);

    bool send(sp<Message> &message);

    vector<sp<Message>> receive();  
    bool isThereMessages() { return (messageBuffer.size()) > 0; };

//    weak_ptr<MessagingUnit> destination;

protected:
    vector<sp<Message>> messageBuffer;
    vector<sp<Message>> pendingBuffer;

    timed_mutex mainMutex;
    // mutex pendingMutex;
    
    // void retreivePending();
    // bool sendToPending(sp<Message> &message);
};

#endif