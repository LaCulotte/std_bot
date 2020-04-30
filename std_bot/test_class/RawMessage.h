#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include "Message.h"

class RawMessage : public Message{
public:
    RawMessage() {};
    virtual ~RawMessage() {};

    virtual void serialize(sp<MessageDataBuffer> output);
    virtual void deserialize(sp<MessageDataBuffer> input);

    MessageDataBuffer data;

    unsigned int getId() { return protocolId; };


    static const unsigned int protocolId = 1;
};

#endif