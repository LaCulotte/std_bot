#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include "NetworkConnection.h"

class ClientConnection : public virtual NetworkConnection {
public: 
    ClientConnection() : NetworkConnection() {connectionKey = " ClientConnection "; };

    virtual bool connectTo(string address, int port);
    
};

#endif