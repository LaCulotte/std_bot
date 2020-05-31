#ifndef PREFIXEDMESSAGE_H
#define PREFIXEDMESSAGE_H

#include "NetworkMessage.h"

class PrefixedMessage : public NetworkMessage {    
public:

    PrefixedMessage() { length = 0; };
    PrefixedMessage(const PrefixedMessage& other) { length = other.length; };

    virtual PrefixedMessage& operator=(const PrefixedMessage& other) { length = other.length; return *this; };
    virtual ~PrefixedMessage() = default;

    // virtual bool deserializePrefix() = 0;
    // virtual bool serializePrefix() = 0;
    // virtual bool deserializePrefix(shared_ptr<MessageDataBuffer> input) { return false; };
    // virtual bool serializePrefix(shared_ptr<MessageDataBuffer> input) { return false; };
    virtual bool deserialize(shared_ptr<MessageDataBuffer> input) override { return false; };
    virtual bool serialize(shared_ptr<MessageDataBuffer> output) override { return false; };


    int getLength() { return length; };
    void setLength(int l) { length = l; }; 

private:

    int length;
};

#endif