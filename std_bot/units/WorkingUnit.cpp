#include "WorkingUnit.h"

sp<thread> WorkingUnit::launch(){
    loopThread = make_shared<thread>(&WorkingUnit::loop, this);
    run = true;
    return loopThread;
}