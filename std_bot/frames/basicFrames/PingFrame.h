#ifndef PINGFRAME_H
#define PINGFRAME_H

#include "Frame.h"
#include "PingMessage.h"
#include "PongMessage.h"

class PingFrame : public Frame {
public:

    virtual bool computeMessage(sp<Message> message, int srcId) override;
};

#endif