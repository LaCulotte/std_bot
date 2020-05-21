#include "WorkingUnit.h"

sp<thread> WorkingUnit::launch(){
    run = true;
    loopThread = make_shared<thread>(&WorkingUnit::loop, this);
    return loopThread;
}