#ifndef CONNECTION_MESSAGE_H
#define CONNECTION_MESSAGE_H

#include "Message.h"

class ConnectionMessage : public Message {
public :
    int sourceConnectionId = -1;
};

#endif