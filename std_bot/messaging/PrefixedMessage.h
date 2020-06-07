#ifndef PREFIXEDMESSAGE_H
#define PREFIXEDMESSAGE_H

#include "NetworkMessage.h"

// Message that use a prefix to identify and annonce its length 
class PrefixedMessage : public NetworkMessage {    
public:

    // Constructor
    PrefixedMessage() { length = 0; };
    // Copy contructor
    PrefixedMessage(const PrefixedMessage& other) { length = other.length; };

    // Copy operator
    virtual PrefixedMessage& operator=(const PrefixedMessage& other) { length = other.length; return *this; };
    // Destructor
    virtual ~PrefixedMessage() = default;

    // Turns raw data into the usable data (message's attributes)
    virtual bool deserialize(shared_ptr<MessageDataBuffer> input) override { return false; };
    // Turns the message's attributes into raw data
    virtual bool serialize(shared_ptr<MessageDataBuffer> output) override { return false; };

    // Returns the length of the message
    int getLength() { return length; };
    // Sets the length of the message
    void setLength(int l) { length = l; }; 

private:

    // Length of the message when it is turned into raw data (excuding the prefix)
    int length;
};

#endif