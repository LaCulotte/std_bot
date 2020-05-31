#ifndef DISCONNECT_FAIL_MESSAGE
#define DISCONNECT_FAIL_MESSAGE

#include "Message.h"

// Tells that the focus was successfully gotten
class DisconnectFailMessage : public Message {
    // Contructor
    DisconnectFailMessage() {};
    // Constructor; with the reason of the failure
    DisconnectFailMessage(string reason) { this->reason = reason; };
    // Copy constuctor
    DisconnectFailMessage(const DisconnectFailMessage& other) = default;

    // Copy operator
    virtual DisconnectFailMessage& operator=(const DisconnectFailMessage& other) = default;
    // Destructor
    virtual ~DisconnectFailMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; };
    // Message's protocol Id
    static const unsigned int protocolId = 623;

    // Reason of the failure
    string reason = "Fail";
};

#endif