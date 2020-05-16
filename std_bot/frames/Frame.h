#ifndef FRAME_H
#define FRAME_H

#include "std_include.h"
#include "ptr_include.h"
#include "Message.h"
#include "MessagingUnit.h"

class MessagingUnit;
class Frame{
public:
    Frame() {};
    Frame(const Frame& other) = default;
    
    virtual ~Frame() = default;
    virtual Frame& operator=(const Frame &other) = default;

    virtual bool computeMessage(sp<Message> message) { return true; };

    MessagingUnit* parent;
    void setPriority(int p) { priority = p; };
    int getPriority() { return priority; }; 

protected:
    // int id;

    //The lower, the latter it will compute messages
    int priority = 100;
};

#define FINISHED_FRAME_H

#endif