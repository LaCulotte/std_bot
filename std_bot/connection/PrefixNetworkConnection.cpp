#include "PrefixNetworkConnection.h"

PrefixNetworkConnection::PrefixNetworkConnection() : NetworkConnection() {
    initHandler();

    connectionKey = "PrefixConnection";
}

PrefixNetworkConnection::PrefixNetworkConnection(const PrefixNetworkConnection& other) : NetworkConnection(other) { 
    msgTypeHandler = other.msgTypeHandler;
}

PrefixNetworkConnection& PrefixNetworkConnection::operator=(const PrefixNetworkConnection& other){
    msgTypeHandler = other.msgTypeHandler;
    return *this;
}

void PrefixNetworkConnection::resetPending(){
    pendingDataFinalLength = 0;
    pendingData.reset();
    pendingMessage.reset();
    pending = false;
}

sp<ConnectionMessage> PrefixNetworkConnection::readMessage() {
    if(!pending){
        sp<MessageDataBuffer> prefixData = readPrefix();

        if(prefixData){
            // TODO : CHANGER : c'est la connection qui va définir comment serialize ou deserialize le prefix => bcp plus pratique ?? (oui je pense)

            // sp<PrefixedMessage> message = dynamic_pointer_cast<PrefixedMessage(msgTypeHandler.)
            sp<PrefixedMessage> message = deserializePrefix(prefixData);
            if(!message){
                Logger::write("The prefix read is not compatible with the type of PrefixedMessage used.", LOG_ERROR);
                Logger::write("Emptying socket buffer. Future data may be messed up", LOG_ERROR);

                emptySocketBuffer();

                return nullptr;
            }

            // sp<Message> typedMessage = msgTypeHandler->generateMessageById(message->getId());
            // if(!typedMessage){
            //     Logger::write("Unkowned message id : '" + to_string(message->getId()) + "'; length : " + to_string(message->getLength()) + "b. Data will be read but not processed.");
            //     sp<MessageDataBuffer> contentData = readData(message->getLength());
            //     if(contentData) {
            //         // TODO : Log contentdata properly
            //         contentData->print();
            //     }

            //     return nullptr;
            // }

            // message = dynamic_pointer_cast<PrefixedMessage>(typedMessage);

            // if(!message){
            //     Logger::write("The message type returned by the MessageTypeHandler is not supported by this PrefixNetworkConnection!", LOG_ERROR);
            //     //TODO : Write actual returned type and current supported type message
            //     return nullptr;
            // }

            if(message->getLength() > 0) {
                sp<MessageDataBuffer> contentData = readData(message->getLength());

                if(!contentData){
                    Logger::write("Error while reading content data for a message. Message id : " + to_string(message->getId()) + "; length : " + to_string(message->getLength()) + "b.");
                    return nullptr;
                }

                if(contentData->size() < message->getLength()){
                    pending = true;
                    pendingData = contentData;
                    pendingDataFinalLength = message->getLength();
                    pendingMessage = message;
                    timeLastPendingUpdate = chrono::system_clock::now();

                    Logger::write("Message too long; Connection state switched to pending");
                    return nullptr;
                }

                if(!message->deserialize(contentData)){
                    Logger::write("Could not deserialize data to message. Message id : " + to_string(message->getLength()) + "; length : " + to_string(message->getLength()) + "b.");
                    //TODO : log message data

                    return nullptr;
                }
            }

            return message;
        } else {
            return nullptr;
        }

    } else {
        auto timeBetweenPendingUpdates = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - timeLastPendingUpdate);
        if(!maxTimeBetweenPendingUpdates || timeBetweenPendingUpdates < *maxTimeBetweenPendingUpdates){
            int rLength = pendingDataFinalLength - pendingData->size();

            sp<MessageDataBuffer> contentData = readData(rLength);

            if(!contentData)
                return nullptr;

            pendingData->push(contentData->getData());

            //VERIFIER SI C'EST BON ET SI IL Y A TOUT !!

            if(contentData->size() == rLength){
                pendingMessage->deserialize(pendingData);

                sp<PrefixedMessage> rMessage = pendingMessage;    
                resetPending();

                return rMessage;
            } else {
                timeLastPendingUpdate = chrono::system_clock::now();

                return nullptr;
            }
                
        } else {
            Logger::write("Message could not be read entirely, 'low' data did not arrive. Received data length : " + to_string(pendingData->size()) + "; Expected data length : " + to_string(pendingDataFinalLength) + "; Message id " + to_string(pendingMessage->getId()) + ".", LOG_IMPORTANT_WARNING);
            //TODO : decide : Log message data ?

            resetPending();
            return readMessage();
        }
    }
}

vector<sp<ConnectionMessage>> PrefixNetworkConnection::readMessages(int n) {
    vector<sp<ConnectionMessage>> ret;

    sp<ConnectionMessage> msg = readMessage();
    while(msg && ret.size() < n){
        ret.push_back(msg);
        msg = readMessage();
    }

    for(int i = ret.size(); i < n; i++){
        ret.push_back(nullptr);
    }

    return ret;
}

vector<sp<ConnectionMessage>> PrefixNetworkConnection::readAllMessages() {
    vector<sp<ConnectionMessage>> ret;

    sp<ConnectionMessage> msg = readMessage();
    while(msg){
        ret.push_back(msg);
        msg = readMessage();
    }

    return ret;
}

//TODO : à enlever ou à implémenter de manière simple et indiquer qu'il faut masquer cette fonction
bool PrefixNetworkConnection::sendMessage(sp<ConnectionMessage> message) {
    sp<PrefixedMessage> prMessage = dynamic_pointer_cast<PrefixedMessage>(message);
    if(!prMessage)
        return false;

    sp<MessageDataBuffer> output(new MessageDataBuffer());

    prMessage->serialize(output);

    return writeData(output);
}