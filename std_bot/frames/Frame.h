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

    virtual bool computeMessage(sp<Message> message, int srcKey) { return true; };

    virtual bool setParent(MessagingUnit* p) { parent = p; return true; };
    void resetParent() { parent = nullptr; };
    MessagingUnit * getParent() { return parent; };

    void setPriority(int p) { priority = p; };
    int getPriority() { return priority; }; 

protected:
    MessagingUnit* parent = nullptr;
    // int id;

    // The lower, the latter it will compute messages 
    // TODO : check si c'est pas l'inverse; check si possibilité d'être négatif (si oui le mentionner)
    int priority = 0;
};

#define FINISHED_FRAME_H

#endif