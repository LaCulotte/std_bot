#ifndef GETCONNECTIONFOCUS_REQUEST_MESSAGE
#define GETCONNECTIONFOCUS_REQUEST_MESSAGE

#include "Message.h"
#include "Connection.h"
#include <functional>

// Asks for the focus of one or multiple connections.
// In other words, it asks some connections to send the incoming messages to the sending MessagingUnit
class GetConnectionFocusRequestMessage : public Message {
    // Contructor
    GetConnectionFocusRequestMessage() {};
    // Constructor; will get the focus from the connection whose id is 'connectionId'
    GetConnectionFocusRequestMessage(int connectionId) { this->connectionId = connectionId; };
    // Constructor; get focus of all the connections that make 'filter' return true
    GetConnectionFocusRequestMessage(function<bool(sp<Connection>)> filter) { this->filter = filter; };
    // Copy constuctor
    GetConnectionFocusRequestMessage(const GetConnectionFocusRequestMessage& other) = default;

    // Copy operator
    virtual GetConnectionFocusRequestMessage& operator=(const GetConnectionFocusRequestMessage& other) = default;
    // Destructor
    virtual ~GetConnectionFocusRequestMessage();

    // Protocol id getter
    unsigned int getId() override { return protocolId; };
    // Message's protocol Id
    static const unsigned int protocolId = 618;

    // Id of the connection to get the focus from
    int connectionId = -1;
    // Filter : get the focus of the connections that make 'filter' return true when passed as argument
    function<bool(sp<Connection>)> filter;

    // True if all the connections passing the filter should change focus
    bool all = false;
    // If 'all = false', the number of connections that shoulld change focus
    int n = 1;
};

#endif