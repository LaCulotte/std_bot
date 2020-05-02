#ifndef PENDINGNETWORKCONNECTION_H
#define PENDINGNETWORKCONNECTION_H

#include "NetworkConnection.h"
#include "PrefixedMessage.h"
#include "Logger.h"

// This class is made to work with PrefixedMessages
// The prefix template is common to all the messages handled by this connection
// Additionnaly, this class implements the mechanism of pending messages
// When the content of a PrefixedMessage is send throught multiple packet,
//  not all the data can be read at once. The connection saves the read data and returns no message.
//  On the next time a message is request, it will try to read the rest of the message.
// This mechanism loops until all the content is read or until the rest of the data does not arrives.
// The latter is configured through the "maxTimeBetweenPendingUpdates" variable. 

template <typename PrMsgT> // Have to be derived of PrefixedMessage
class PrefixNetworkConnection : public virtual NetworkConnection {
public:

    PrefixNetworkConnection() : NetworkConnection();
    PrefixNetworkConnection(const PrefixNetworkConnection& other) : NetworkConnection(other);

    //Tester que ca appelle la fonction surchargée
    virtual PrefixNetworkConnection& operator=(const PrefixNetworkConnection& other);
    virtual ~PrefixNetworkConnection() = default;


    virtual sp<Message> readMessage() override;
    // virtual vector<sp<Message>> readMessages(int n) override {};
    // virtual vector<sp<Message>> readAllMessages() override {};

protected :

    virtual sp<MessageDataBuffer> readPrefix() = 0;

    // If the data requested is bigger than the data contained in the socket read buffer, the 
    // NetworkConnection with save the read data and free the current thread. The next time data is requested,
    // the connection will continue from the saved data, until the length requested is read or until an other 
    // message is requested.
    bool pending = false;
    sp<MessageDataBuffer> pendingData;
    int pendingDataFinalLength;
    sp<PrMsgT> pendingMessage;
    timeval timeLastPendingUpdate;
    timeval *maxTimeBetweenPendingUpdates;
};

template<typename PrMsgT>
PrefixNetworkConnection<PrMsgT>::PrefixNetworkConnection(){
    static_assert(is_base_of<PrefixedMessage, PrMsgT>::value, "PrMsgT must be derived of PrefixedMessage.");
}

template<typename PrMsgT>
PrefixNetworkConnection<PrMsgT>::PrefixNetworkConnection(const PrefixNetworkConnection& other){
    static_assert(is_base_of<PrefixedMessage, PrMsgT>::value, "PrMsgT must be derived of PrefixedMessage.");
}

template<typename PrMsgT>
sp<Message> PrefixNetworkConnection<PrMsgT>::readMessage(){
    if(!pending){
        sp<MessageDataBuffer> prefixData = readPrefix();

        if(prefixData){
            // sp<PrefixedMessage> message = dynamic_pointer_cast<PrefixedMessage(msgTypeHandler.)
            sp<PrMsgT> message();
            if(!message->deserializePrefix(prefixData)){
                Logger::write("The prefix read is not compatible with the type of PrefixedMessage used.", LOG_ERROR);
                Logger::write("Emptying socket buffer. Future data may be messed up", LOG_ERROR);

                emptySocketBuffer();

                return nullptr;
            }

            sp<Message> typedMessage = msgTypeHandler->getTypeById(message->getId());
            if(!typedMessage){
                Logger::write("Unkowned message id : '" + to_string(message->getId()) "'; length : " + message->getLength() + "b. Data will be read but not processed.")
                sp<MessageDataBuffer> contentData = readData(message->getLength())
                if(contentData) {
                    // TODO : Log contentdataProperly
                    contentData.print();
                }

                return nullptr;
            }

            message = dynamic_pointer_cast<PrMsg>(typedMessage);

            if(!message){
                Logger::write("The message type returned by the MessageTypeHandler is not supported by this PrefixNetworkConnection!", LOG_ERROR);
                //TODO : Write actual returned type and current supported type message
                return nullptr;
            }

            sp<MessageDataBuffer> contentData = readData(message->getLength());

            if(!contentData){
                Logger::write("Error while reading content data for a message. Message id : " + to_string(message->getId()) + "; length : " + to_string(message->getLength()) + "b.");
                return nullptr;
            }

            if(contentData.size() < message->getLength()){
                pending = true;
                pendingData = contentData;
                pendingDataFinalLength = message->getLength();
                pendingMessage = message;
                //TODO : timeLastPendingUpdate = now()

                Logger::write("Message too long; Connection switched to pending");
                return nullptr;
            }

            if(!message->deserialize(contentData)){
                Logger::write("Could not deserialize data to message. Message id : " + to_string(message->getLength) + "; length : " + to_string(message->getLength()) + "b.");
                //TODO : log message data

                return nullptr;
            }

            return message;
        } else {
            return nullptr;
        }

    } else {
        return nullptr;
    }
}

#endif