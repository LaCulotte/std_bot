#ifndef PREFIXEDMESSAGE_H
#define PREFIXEDMESSAGE_H

#include "Message.h"

class PrefixedMessage : public Message {    
public:

    PrefixedMessage() { length = 0; };
    PrefixedMessage(const PrefixedMessage& other) { length = other.length; };

    virtual PrefixedMessage& operator=(const PrefixedMessage& other) { length = other.length; return *this; };
    virtual ~PrefixedMessage() = default;

    virtual bool deserializePrefix() = 0;
    virtual bool serializePrefix() = 0;

    int getLength() { return length; };

private:

    int length;
};

#endif