#ifndef FRAME_H
#define FRAME_H

#include "std_include.h"
#include "ptr_include.h"
#include "Message.h"

class Frame{
public:
    Frame() {};
    Frame(const Frame& other) = default;
    
    virtual ~Frame() = default;
    virtual Frame& operator=(const Frame &other) = default;

    virtual bool computeMessage(sp<Message> message) { return true; };

private:
    int id;
};

#endif