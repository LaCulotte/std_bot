#ifndef DISCONNECT_SUCCESS_MESSAGE
#define DISCONNECT_SUCCESS_MESSAGE

#include "Message.h"

// Tells that the focus was successfully gotten
class DisconnectSuccessMessage : public Message {
    // Contructor
    DisconnectSuccessMessage() {};
    // Constructor; with the ids of the connection that was disconnected
    DisconnectSuccessMessage(vector<int> connectionIds) { this->connectionIds = connectionIds; };
    // Copy constuctor
    DisconnectSuccessMessage(const DisconnectSuccessMessage& other) = default;

    // Copy operator
    virtual DisconnectSuccessMessage& operator=(const DisconnectSuccessMessage& other) = default;
    // Destructor
    virtual ~DisconnectSuccessMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; };
    // Message's protocol Id
    static const unsigned int protocolId = 622;

    // Ids of the connection that was disconnected
    vector<int> connectionIds = {};
};

#endif