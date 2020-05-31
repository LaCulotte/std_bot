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
  
    virtual unsigned int getId() { return protocolId; };

    static const unsigned int protocolId = 0;
    
    bool keepInLoop = false;
    int lifetime = 0;
};

#endif