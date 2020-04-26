#ifndef DOFUSNETWORK_MESSAGE_H
#define DOFUSNETWORK_MESSAGE_H

#include "Message.h"

/*Basically a Token-class that indicates that it is a
* standard dofus message, and that the connection should
* add the standard header
*/

class DofusNetworkMessage : Message { 
public:

    DofusNetworkMessage() {};
    DofusNetworkMessage(const DofusNetworkMessage& other) = default;

    virtual DofusNetworkMessage& operator=(const DofusNetworkMessage& other) = default;
    virtual ~DofusNetworkMessage() = default;
  

};

#endif 