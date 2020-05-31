#ifndef PING_MESSAGE_H
#define PING_MESSAGE_H

#include "Message.h"

class PingMessage : public Message {
public: 
    virtual unsigned int getId() override { return protocolId; };
    static const unsigned int protocolId = 600;
};

#endif