#ifndef PREFIXNETWORKCONNECTION_H
#define PREFIXNETWORKCONNECTION_H

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

class PrefixNetworkConnection : public virtual NetworkConnection {
public:

    PrefixNetworkConnection();
    PrefixNetworkConnection(const PrefixNetworkConnection& other);

    //Tester que ca appelle la fonction surchargée
    virtual PrefixNetworkConnection& operator=(const PrefixNetworkConnection& other);
    virtual ~PrefixNetworkConnection() = default;

    virtual sp<ConnectionMessage> readMessage() override;
    virtual vector<sp<ConnectionMessage>> readMessages(int n) override;
    virtual vector<sp<ConnectionMessage>> readAllMessages() override;

    virtual bool sendMessage(sp<ConnectionMessage> message) override;

// protected:
public:
    //TODO : rendre purement virtuelles
    virtual sp<MessageDataBuffer> readPrefix() { return nullptr; };
    virtual sp<PrefixedMessage> deserializePrefix(sp<MessageDataBuffer> data) { return nullptr; };
    virtual bool serializePrefix(sp<MessageDataBuffer> data, sp<PrefixedMessage> msg) = 0;

    void resetPending();

    // If the data requested is bigger than the data contained in the socket read buffer, the 
    // NetworkConnection with save the read data and free the current thread. The next time data is requested,
    // the connection will continue from the saved data, until the length requested is read or until an other 
    // message is requested.
    bool pending = false;
    sp<MessageDataBuffer> pendingData;
    int pendingDataFinalLength;
    sp<PrefixedMessage> pendingMessage;
    chrono::system_clock::time_point timeLastPendingUpdate;
    // Maximum time between two following packets so they can be considered from the same message
    //  nullptr means that two packets can be infinitly appart and still can be consederes from the same message
    chrono::system_clock::duration *maxTimeBetweenPendingUpdates;
};

#endif