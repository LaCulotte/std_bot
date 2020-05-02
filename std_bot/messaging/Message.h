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
  


    virtual bool serialize(shared_ptr<MessageDataBuffer> output) = 0;
    virtual bool deserialize(shared_ptr<MessageDataBuffer> input) = 0;

    virtual unsigned int getId() { return protocolId; };

    static const unsigned int protocolId = 0;
    
};

#endif