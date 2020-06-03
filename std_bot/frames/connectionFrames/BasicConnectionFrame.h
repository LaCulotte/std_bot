#ifndef BASICCONNECTION_FRAME_H
#define BASICCONNECTION_FRAME_H

#include "Frame.h"
#include "ConnectionUnit.h"

#include "SendPacketRequestMessage.h"
#include "SendPacketSuccessMessage.h"
#include "SendPacketFailureMessage.h"
#include "GetConnectionFocusRequestMessage.h"
#include "GetConnectionFocusSuccessMessage.h"
#include "GetConnectionFocusFailureMessage.h"
#include "ConnectionRequestMessage.h"
#include "ConnectionSuccessMessage.h"
#include "ConnectionFailureMessage.h"
#include "ConnectionIdGetMessage.h"
#include "ConnectionIdMessage.h"
#include "DisconnectRequestMessage.h"
#include "DisconnectedMessage.h"
// #include "DisconnectFailureMessage.h"

class BasicConnectionFrame : public Frame {
public:

    virtual bool setParent(MessagingUnit* parent) override;

    virtual bool computeMessage(sp<Message> message, int srcId) override;

};

#endif