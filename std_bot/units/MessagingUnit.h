#ifndef MESSAGINGUNIT_H
#define MESSAGINGUNIT_H

#include "WorkingUnit.h"
#include "MessageInterface.h"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include "UpdatingFrame.h"

class Frame;
class UpdatingFrame;

// Does not mean that it communicates throught network. It communicates with other MessagingUnits
class MessagingUnit : public WorkingUnit {
public :
    MessagingUnit();
    MessagingUnit(const MessagingUnit& other) = default;

    virtual MessagingUnit& operator=(const MessagingUnit& other) = default;
    virtual ~MessagingUnit();

    void initFrames() {};
    bool addFrame(sp<Frame> frame, bool alwaysUpdates = false);

    template<typename T>
    sp<Frame> getFrame();
    template<typename T>
    vector<sp<Frame>> getFrames(int n);
    template<typename T>
    vector<sp<Frame>> getAllFrames();

    sp<Frame> removeFrame(sp<Frame> frame);
    sp<Frame> removeFrame(Frame * frame);
    vector<sp<Frame>> removeFrames(vector<sp<Frame>> frames, bool with_nullptr = true);
    vector<Frame *> removeFrames(vector<Frame *> frames, bool with_nullptr = true);

    template<typename T>
    sp<Frame> popFrame();
    template<typename T>
    vector<sp<Frame>> popFrames(int n);
    template<typename T>
    vector<sp<Frame>> popAllFrames();

    static bool link(sp<MessagingUnit> mu1, sp<MessagingUnit> mu2);

    bool sendSelfMessage(sp<Message> message);
    bool sendMessage(sp<Message> message, string destKey);

    sp<MessageInterface> msgInterface_in;
protected:

    virtual void tick() override;

    multimap<int, sp<Frame>> frames;
    unordered_set<sp<Frame>> addedFrames;
    vector<sp<UpdatingFrame>> alwaysUptadingFrames;  
    void removeFrameFromArrays(multimap<int, sp<Frame>>::iterator itr);  

    vector<sp<Message>> messagesToProcess;

    int maxMessageLifetime = 1;

    bool canAddMessagingUnit(sp<MessagingUnit> otherMU);
    bool addMessagingUnit(sp<MessagingUnit> otherMU);
    unordered_map<string, weak_ptr<MessageInterface>> msgInterfaces_out;

    void retreiveMessages();

    string messagingUnitKey = "BasicMessagingUnit";

    void initSelfPointer();
    sp<MessagingUnit> selfPointer;
};

template<typename T>
sp<Frame> MessagingUnit::popFrame(){
    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++){
        if(dynamic_pointer_cast<T>(it->second)) {
            removeFrameFromArrays(it);
            return it->second;
        }
    }

    return nullptr;
}

template<typename T>
vector<sp<Frame>> MessagingUnit::popFrames(int n){
    vector<multimap<int, sp<Frame>>::iterator> delIterators;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end() && delIterators.size() < n; it++){
        if(dynamic_pointer_cast<T>(it->second)) {
            delIterators.push_back(it);
        }
    }

    vector<sp<Frame>> ret;
    for(auto it : delIterators){
        removeFrameFromArrays(it);
        ret.push_back(it->second);
    }

    while(ret.size() < n)
        ret.push_back(nullptr);

    return ret;
}

template<typename T>
vector<sp<Frame>> MessagingUnit::popAllFrames(){
    vector<multimap<int, sp<Frame>>::iterator> delIterators;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++){
        if(dynamic_pointer_cast<T>(it->second)) {
            delIterators.push_back(it);
        }
    }

    vector<sp<Frame>> ret;
    for(auto it : delIterators){
        removeFrameFromArrays(it);
        ret.push_back(it->second);
    }

    return ret;
}

template<typename T>
sp<Frame> MessagingUnit::getFrame(){
    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++) {
        if(dynamic_pointer_cast<T>(it->second))
            return it->second;
    }

    return nullptr;
}

template<typename T>
vector<sp<Frame>> MessagingUnit::getFrames(int n){
    vector<sp<Frame>> ret;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end() && ret.size() < n; it++) {
        if(dynamic_pointer_cast<T>(it->second))
            ret.push_back(it->second);
    }

    while(ret.size() < n)
        ret.push_back(nullptr);

    return ret;
}

template<typename T>
vector<sp<Frame>> MessagingUnit::getAllFrames(){
    vector<sp<Frame>> ret;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++) {
        if(dynamic_pointer_cast<T>(it->second))
            ret.push_back(it->second);
    }

    return ret;
}

#endif
