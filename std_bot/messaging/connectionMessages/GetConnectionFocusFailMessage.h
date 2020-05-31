#ifndef GETCONNECTIONFOCUS_FAIL_MESSAGE
#define GETCONNECTIONFOCUS_FAIL_MESSAGE

#include "Message.h"

// Tells that the focus was successfully gotten
class GetConnectionFocusFailMessage : public Message {
    // Contructor
    GetConnectionFocusFailMessage() {};
    // Constructor; with the reason of the failure
    GetConnectionFocusFailMessage(string reason) { this->reason = reason; };
    // Copy constuctor
    GetConnectionFocusFailMessage(const GetConnectionFocusFailMessage& other) = default;

    // Copy operator
    virtual GetConnectionFocusFailMessage& operator=(const GetConnectionFocusFailMessage& other) = default;
    // Destructor
    virtual ~GetConnectionFocusFailMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; };
    // Message's protocol Id
    static const unsigned int protocolId = 620;

    // Reason of the failure
    string reason = "Fail";
};

#endif