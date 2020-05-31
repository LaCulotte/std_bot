#ifndef CONNECTIONUNIT_H
#define CONNECTIONUNIT_H

#include "MessagingUnit.h"
#include "Connection.h"
#include "ClientConnection.h"
#include "ServerConnection.h"
//TODO : enlever ce truc
#include "BasicPrefixConnection.h"

class ConnectionUnit : public MessagingUnit {
public:
    ConnectionUnit();
    ConnectionUnit(const ConnectionUnit& other) = default;

    virtual ConnectionUnit& operator=(const ConnectionUnit& other) = default;
    virtual ~ConnectionUnit() = default;

    virtual int connectToServer(string address, int port, int sourceInterfaceKey = -1);
    // int HTTPConnection(string url);
    int addConnection(sp<Connection> connection, int sourceInterfaceKey = -1);

    bool sendConnectionMessage(sp<ConnectionMessage> message, int destConnectionKey, int sourceInterfaceKey = -1);
    bool changeConnectionDestination(int connectionKey, int newDestination);

    void disconnect(int connectionKey);

    template<typename T>
    int getConnectionId();
    template<typename T>
    int getConnectionId(bool filter(T));
    template<typename T>
    vector<int> getAllConnectionIds();
    template<typename T>
    vector<int> getAllConnectionIds(bool filter(T));

    virtual void initFrames() override;
    virtual void tick() override;

    virtual bool removeMessageInterface(int interfaceKey) override;
    virtual bool removeMessageInterface(unordered_map<int, sp<MessageInterface>*>::iterator interfaceIt) override;

protected:

    unordered_map<int, sp<Connection>> connections;
    int lastConnectionId = 0;

    unordered_map<int, int> connectionKey_to_interfaceKey;
    // Changes when : sendMsg, connectToNewConnection, changeConnectionDestinationToIt
    unordered_map<int, int> interfaceKey_to_preferredConnectionKey;
};

template<typename T>
int ConnectionUnit::getConnectionId(){
    for(auto it = connections.begin(); it != connections.end(); it++){
        if(dynamic_pointer_cast<T>(it->second))
            return it->first;
    }

    return -1;
}

template<typename T>
int ConnectionUnit::getConnectionId(bool filter(T)){
    for(auto it = connections.begin(); it != connections.end(); it++){
        sp<T> T_cast = dynamic_cast<T>(it->second);
        if(T_cast && filter(T_cast))
            return it->first;
    }

    return -1;
}

template<typename T>
vector<int> ConnectionUnit::getAllConnectionIds(){
    vector<int> ret;

    for(auto it = connections.begin(); it != connections.end(); it++){
        if(dynamic_pointer_cast<T>(it->second))
            ret.push_back(it->first);
    }

    return ret;
}

template<typename T>
vector<int> ConnectionUnit::getAllConnectionIds(bool filter(T)){
    vector<int> ret;

    for(auto it = connections.begin(); it != connections.end(); it++){
        sp<T> T_cast = dynamic_cast<T>(it->second);
        if(T_cast && filter(T_cast))
            ret.push_back(it->first);
    }

    return ret;
}

#endif