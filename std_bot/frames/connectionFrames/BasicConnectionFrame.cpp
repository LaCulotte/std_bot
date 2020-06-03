#include "BasicConnectionFrame.h"

bool BasicConnectionFrame::setParent(MessagingUnit *parent) {
    if(dynamic_cast<ConnectionUnit *>(parent))
        return Frame::setParent(parent);

    return false;
}

bool BasicConnectionFrame::computeMessage(sp<Message> message, int srcId) {
    ConnectionUnit *connectionParent = dynamic_cast<ConnectionUnit *>(parent);     

    if(!connectionParent)
        return false;

    sp<SendPacketRequestMessage> sprMsg;
    sp<GetConnectionFocusRequestMessage> gcfrMsg;
    sp<ConnectionRequestMessage> crMsg;
    sp<ConnectionIdGetMessage> cigMsg;
    sp<ConnectionIdMessage> ciMsg;
    sp<DisconnectRequestMessage> drMsg;

    switch (message->getId())
    {
    case SendPacketRequestMessage::protocolId:
        sprMsg = dynamic_pointer_cast<SendPacketRequestMessage>(message);
        
        if(connectionParent->sendConnectionMessage(sprMsg->message, sprMsg->connectionId, srcId)){
            connectionParent->sendMessage(make_shared<SendPacketSuccessMessage>(), srcId);
        } else {
            connectionParent->sendMessage(make_shared<SendPacketFailureMessage>(), srcId);
        }
        
        return true;

    case GetConnectionFocusRequestMessage::protocolId:
        gcfrMsg = dynamic_pointer_cast<GetConnectionFocusRequestMessage>(message);

        if(gcfrMsg->connectionIds.size()) {
            vector<int> successIds;
            for(int id : gcfrMsg->connectionIds){
                if(connectionParent->changeConnectionDestination(id, srcId)){
                    successIds.push_back(id);
                }
            }
            if(!successIds.size()){
                connectionParent->sendMessage(make_shared<GetConnectionFocusFailureMessage>("All failed"), srcId);
            } else {
                connectionParent->sendMessage(make_shared<GetConnectionFocusSuccessMessage>(successIds), srcId);
            }

        } else if(gcfrMsg->filter) {
            vector<int> ids;
            vector<int> successIds;
            if(gcfrMsg->all){
                ids = connectionParent->getAllConnectionIds<Connection>(gcfrMsg->filter);
            } else {
                ids = connectionParent->getConnectionIds<Connection>(gcfrMsg->n, gcfrMsg->filter);
            }

            for(int id : ids){
                if(connectionParent->changeConnectionDestination(id, srcId)){
                    successIds.push_back(id);
                }
            }

            if(!successIds.size()){
                connectionParent->sendMessage(make_shared<GetConnectionFocusFailureMessage>("No match"), srcId);
            } else {
                connectionParent->sendMessage(make_shared<GetConnectionFocusSuccessMessage>(successIds), srcId);
            }
        }

        return true;

    case ConnectionRequestMessage::protocolId:
        crMsg = dynamic_pointer_cast<ConnectionRequestMessage>(message);

        if(crMsg->toConnect){
            if(crMsg->toConnect->connectTo(crMsg->address, crMsg->port)){
                int n_id = connectionParent->addConnection(crMsg->toConnect, srcId);
                connectionParent->sendMessage(make_shared<ConnectionSuccessMessage>(n_id), srcId);
            } else {
                connectionParent->sendMessage(make_shared<ConnectionFailureMessage>("Connection fail"), srcId);
            }
        } else {
            sp<ClientConnection> toConnect = crMsg->connectionBuilder();
            cout << "toConnect >= BasicPrefixedCOnnection? : " << dynamic_pointer_cast<BasicPrefixConnection>(toConnect) << endl;
            if(toConnect){
                if(toConnect->connectTo(crMsg->address, crMsg->port)){
                    int n_id = connectionParent->addConnection(toConnect, srcId);
                    connectionParent->sendMessage(make_shared<ConnectionSuccessMessage>(n_id), srcId);
                } else {
                    connectionParent->sendMessage(make_shared<ConnectionFailureMessage>("Connection fail"), srcId);
                }
            } else {
                connectionParent->sendMessage(make_shared<ConnectionFailureMessage>("Build fail"), srcId);
            }
        }
        return true;

    case ConnectionIdGetMessage::protocolId:
        cigMsg = dynamic_pointer_cast<ConnectionIdGetMessage>(message);
        ciMsg = make_shared<ConnectionIdMessage>();
        if(cigMsg->all){
            ciMsg->ids = connectionParent->getAllConnectionIds<Connection>(cigMsg->filter);
        } else {
            ciMsg->ids = connectionParent->getConnectionIds<Connection>(cigMsg->n, cigMsg->filter);
        }
        connectionParent->sendMessage(ciMsg, srcId);

        return true;
    
    case DisconnectRequestMessage::protocolId:
        drMsg = dynamic_pointer_cast<DisconnectRequestMessage>(message);
        
        if(drMsg->connectionIds.size()){
            for(int id : drMsg->connectionIds){
                connectionParent->disconnect(id);
            }

            parent->sendMessage(make_shared<DisconnectedMessage>(drMsg->connectionIds), srcId);
        } else {
            vector<int> disconnectIds;
            if(drMsg->all){
                disconnectIds = connectionParent->getAllConnectionIds<Connection>(drMsg->filter);
            } else {
                disconnectIds = connectionParent->getConnectionIds<Connection>(drMsg->n, drMsg->filter);
            }

            for(int id : disconnectIds)
                connectionParent->disconnect(id);

            parent->sendMessage(make_shared<DisconnectedMessage>(disconnectIds), srcId);
        }

        return true;
    }

    return false;
}