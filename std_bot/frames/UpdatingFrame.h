#ifndef UPDATINGFRAME_H
#define UPDATINGFRAME_H

#include "Frame.h"

class UpdatingFrame : public Frame{
public:
    UpdatingFrame() {};
    UpdatingFrame(const UpdatingFrame& other) = default;
    
    virtual ~UpdatingFrame() = default;
    virtual UpdatingFrame& operator=(const UpdatingFrame &other) = default;


    virtual bool update() { return true; };

protected:
    bool alwaysUpdate = false;
};

#endif