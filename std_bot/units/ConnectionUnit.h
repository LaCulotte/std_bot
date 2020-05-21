#ifndef CONNECTIONUNIT_H
#define CONNECTIONUNIT_H

#include "MessagingUnit.h"
#include "Connection.h"
#include "ClientConnection.h"
#include "ServerConnection.h"

class ConnectionUnit : public MessagingUnit {
public:
    ConnectionUnit();
    ConnectionUnit(const ConnectionUnit& other) = default;

    virtual ConnectionUnit& operator=(const ConnectionUnit& other) = default;
    virtual ~ConnectionUnit() = default;

    int connectToServer(string address, int port);
    // int HTTPConnection(string url);
    int addConnection(sp<Connection> connection);

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

protected:

    unordered_map<int, sp<Connection>> connections;
    int lastConnectionId = 0;

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