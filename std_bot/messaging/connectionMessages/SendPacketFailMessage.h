#ifndef SENDPACKET_FAIL_MESSAGE_H
#define SENDPACKET_FAIL_MESSAGE_H

#include "Message.h"

// Used to tell that the requested packet was not sent, with a reason defaulted at "Fail"
class SendPacketFailMessage : public Message {
public:
    // Constructor
    SendPacketFailMessage() {};
    // Constructor with reason setter
    SendPacketFailMessage(string reason) { this->reason = reason; };
    // Copy construtor
    SendPacketFailMessage(const SendPacketFailMessage& other) = default;


    // Copy operator
    virtual SendPacketFailMessage& operator=(const SendPacketFailMessage& other) = default;
    // Destructor
    virtual ~SendPacketFailMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; }
    // Message's protocol Id
    static const unsigned int protocolId = 612;

    // Reason that the packet send was not successful
    string reason = "Fail";
};

#endif