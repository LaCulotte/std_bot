#ifndef FROMCONNECTIONMESSAGE_H
#define FROMCONNECTIONMESSAGE_H

#include "ConnectionMessage.h"

// Type of message used to recover the ConnectionUnit's connection key
class NetworkMessage : public ConnectionMessage {
public:
    virtual bool deserialize(shared_ptr<MessageDataBuffer> input) = 0;
    virtual bool serialize(shared_ptr<MessageDataBuffer> output) = 0;
};

#endif