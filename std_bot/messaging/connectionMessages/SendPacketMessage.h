#ifndef SENDPACKETMESSAGE_H
#define SENDPACKETMESSAGE_H

#include "ConnectionMessage.h"

// Used to tell the ConnectionUnit to send a message
class SendPacketMessage : public Message {
public:
    // Contructor
    SendPacketMessage() {};
    // Constructor
    SendPacketMessage(sp<ConnectionMessage> message) { this->message = message; };
    // Constructor
    SendPacketMessage(sp<ConnectionMessage> message, int connectionId) { this->message = message; this->connectionId = connectionId; };
    // Copy constuctor
    SendPacketMessage(const SendPacketMessage& other) { message = make_shared<ConnectionMessage>(); *message = *other.message; };

    // Copy operator
    virtual SendPacketMessage& operator=(const SendPacketMessage& other) { message = make_shared<ConnectionMessage>(); *message = *other.message; return *this; };
    // Destructor
    virtual ~SendPacketMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; };
    // Message's protocol Id
    static const unsigned int protocolId = 610;

    // Packet to send through the ConnectionUnit
    sp<ConnectionMessage> message; 
    // Connection through witch the packet will be sent
    int connectionId = -1;
    //TODO :connectionId?
};

#endif