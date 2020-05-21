#ifndef WORKINGUNIT_H
#define WORKINGUNIT_H

#include "Logger.h"
#include "thread_include.h"
#include "ptr_include.h"

class WorkingUnit {
public:
    WorkingUnit() {};
    WorkingUnit(const WorkingUnit& other) = default;

    virtual WorkingUnit& operator=(const WorkingUnit& other) = default;
    virtual ~WorkingUnit() = default;

    virtual sp<thread> launch();
    virtual void stop() { run = false; };
    virtual void waitThreadEnd() { loopThread->join(); };

protected:
    sp<thread> loopThread;

    bool run = false;

    virtual void loop() { while(run) tick(); };
    virtual void tick() {};
};

#endif