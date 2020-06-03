#include "ConnectionUnit.h"

ConnectionUnit::ConnectionUnit() : MessagingUnit() {
    messagingUnitKey = "ConnectionUnit";
}

void ConnectionUnit::initFrames(){
    addFrame(make_shared<BasicConnectionFrame>());
}

int ConnectionUnit::connectToServer(string address, int port, int sourceInterfaceKey){
    //TODO : Remettre une clientConnectionHabituelle
    sp<BasicPrefixConnection> newConnection(new BasicPrefixConnection());

    if(newConnection->connectTo(address, port)){
        return addConnection(newConnection, sourceInterfaceKey);
    } else {
        return -1;
    }
}

int ConnectionUnit::addConnection(sp<Connection> connection, int sourceInterfaceKey){
    if(!connection)
        return -1;

    connections.insert(pair<int, sp<Connection>>(lastConnectionId++, connection));

    if(sourceInterfaceKey != -1 && msgInterfaces.find(sourceInterfaceKey) != msgInterfaces.end()) {
        
        connectionKey_to_interfaceKey.insert(pair(lastConnectionId - 1, sourceInterfaceKey));

        interfaceKey_to_preferredConnectionKey[sourceInterfaceKey] = lastConnectionId - 1;
    }

    return lastConnectionId - 1;
}

bool ConnectionUnit::sendConnectionMessage(sp<ConnectionMessage> message, int destConnectionKey, int sourceInterfaceKey) {
    if(connections.find(destConnectionKey) != connections.end()){
        sp<Connection> connection = connections.at(destConnectionKey);

        if(msgInterfaces.find(sourceInterfaceKey) != msgInterfaces.end())
            interfaceKey_to_preferredConnectionKey[sourceInterfaceKey] = destConnectionKey;

        return connection->sendMessage(message);
    } else if(interfaceKey_to_preferredConnectionKey.find(sourceInterfaceKey) != interfaceKey_to_preferredConnectionKey.end()){
        int connectionKey = interfaceKey_to_preferredConnectionKey.at(sourceInterfaceKey);

        if(connections.find(connectionKey) != connections.end())
            return connections.at(connectionKey)->sendMessage(message);
    }

    return false;
}

bool ConnectionUnit::changeConnectionDestination(int connectionKey, int newDestinationKey) {
    if(connections.find(connectionKey) != connections.end() && msgInterfaces.find(newDestinationKey) != msgInterfaces.end()){

        connectionKey_to_interfaceKey[connectionKey] = newDestinationKey;

        interfaceKey_to_preferredConnectionKey[newDestinationKey] = connectionKey;

        return true;
    }

    return false;
}

void ConnectionUnit::disconnect(int connectionKey){
    if(connections.find(connectionKey) != connections.end()){
        connections.at(connectionKey)->disconnect();

        connections.erase(connectionKey);
        if(connectionKey_to_interfaceKey.find(connectionKey) != connectionKey_to_interfaceKey.end())
            connectionKey_to_interfaceKey.erase(connectionKey); 
        
        //A voir si je le laisse
        for(auto it = interfaceKey_to_preferredConnectionKey.begin(); it != interfaceKey_to_preferredConnectionKey.end(); it++){
            if(it->second = connectionKey)
                interfaceKey_to_preferredConnectionKey.erase(it);
        }
    }
}

void ConnectionUnit::tick(){
    for(auto it = connections.begin(); it != connections.end(); it++){
        sp<ConnectionMessage> message = it->second->readMessage();
        if(!message)
            continue;
        
        message->sourceConnectionId = it->first;

        if(connectionKey_to_interfaceKey.find(it->first) != connectionKey_to_interfaceKey.end()){

            int destKey = connectionKey_to_interfaceKey.at(it->first);
            if(msgInterfaces.find(destKey) != msgInterfaces.end()){
                sp<MessageInterface> dest = msgInterfaces.at(destKey)[1];
                dest->send(message);
            } else {
                connectionKey_to_interfaceKey.erase(it->first);
            }
        }
    }

    MessagingUnit::tick();
}

bool ConnectionUnit::removeMessageInterface(int interfaceKey) {
    if(MessagingUnit::removeMessageInterface(interfaceKey)) {
    
        auto it = interfaceKey_to_preferredConnectionKey.find(interfaceKey);
        if(it != interfaceKey_to_preferredConnectionKey.end()){
            interfaceKey_to_preferredConnectionKey.erase(it);
        }

        return true;
    }

    return false;
}

bool ConnectionUnit::removeMessageInterface(unordered_map<int, sp<MessageInterface>*>::iterator interfaceIt) {
    if(MessagingUnit::removeMessageInterface(interfaceIt)) {
    
        auto it = interfaceKey_to_preferredConnectionKey.find(interfaceIt->first);
        if(it != interfaceKey_to_preferredConnectionKey.end()){
            interfaceKey_to_preferredConnectionKey.erase(it);
        }

        return true;
    }

    return false;
}