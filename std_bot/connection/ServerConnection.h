#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "NetworkConnection.h"

class ServerConnection : public virtual NetworkConnection {
public:
    ServerConnection();
};

#endif