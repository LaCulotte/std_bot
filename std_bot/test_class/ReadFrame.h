#ifndef READFRAME_H
#define READFRAME_H

#include "UpdatingFrame.h"
#include "RawMessage.h"

class ReadFrame : public UpdatingFrame {
public:
    ReadFrame();
    ~ReadFrame();

    bool computeMessage(sp<Message> message);
    bool update();
};

#endif