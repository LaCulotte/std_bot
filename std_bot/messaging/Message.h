#ifndef MESSAGE_H
#define MESSAGE_H

#include "std_include.h"
#include "ptr_include.h"
#include "MessageDataBuffer.h"

using namespace std;
class Message {
public:

    Message() {};
    Message(const Message& other) = default;

    virtual Message& operator=(const Message& other) = default;
    virtual ~Message() = default;
  


    virtual void serialize(shared_ptr<MessageDataBuffer> output) {};
    virtual void deserialize(shared_ptr<MessageDataBuffer> input) {};

    virtual unsigned int getId() { return protocolId; };

    static const unsigned int protocolId = 0;
    
};

#endif