#ifndef CONNECTION_FAIL_MESSAGE_H
#define CONNECTION_FAIL_MESSAGE_H

#include "Message.h"

// Used to tell that the requested packet was not sent, with a reason defaulted at "Fail"
class ConnectionFailMessage : public Message {
public:
    // Constructor
    ConnectionFailMessage() {};
    // Constructor with reason setter
    ConnectionFailMessage(string reason) { this->reason = reason; };
    // Copy construtor
    ConnectionFailMessage(const ConnectionFailMessage& other) = default;


    // Copy operator
    virtual ConnectionFailMessage& operator=(const ConnectionFailMessage& other) = default;
    // Destructor
    virtual ~ConnectionFailMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; }
    // Message's protocol Id
    static const unsigned int protocolId = 615;

    // Reason that the packet send was not successful
    string reason = "Fail";
};

#endif