#include "PingFrame.h"

bool PingFrame::computeMessage(sp<Message> message, int srcId){
    if(!parent)
        return false;

    if(message->getId() == PingMessage::protocolId){
        parent->sendMessage(make_shared<PongMessage>(), srcId);
        return true;
    }

    return false;
}