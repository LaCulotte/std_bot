#include "ConnectionUnit.h"

ConnectionUnit::ConnectionUnit() : MessagingUnit() {
}

void ConnectionUnit::initFrames(){

}

int ConnectionUnit::connectToServer(string address, int port){
    sp<ClientConnection> newConnection(new ClientConnection());

    if(newConnection->connectTo(address, port)){
        return addConnection(newConnection);
    } else {
        return -1;
    }
}

int ConnectionUnit::addConnection(sp<Connection> connection){
    if(!connection)
        return -1;

    connections.insert(pair<int, sp<Connection>>(lastMsgInterfaceOutId++, connection));

    return lastMsgInterfaceOutId - 1;
}

void ConnectionUnit::tick(){
    
}