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
class MessageInterface;

// Does not mean that it communicates throught network. It communicates with other MessagingUnits
class MessagingUnit : public WorkingUnit {
public :
    MessagingUnit();
    MessagingUnit(const MessagingUnit& other) = default;

    virtual MessagingUnit& operator=(const MessagingUnit& other) = default;
    virtual ~MessagingUnit();

    virtual void initFrames() {};
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
    bool sendMessage(sp<Message> message, int destId);

    template<typename T>
    int getMessageInterfaceOutId();
    template<typename T>
    int getMessageInterfaceOutId(bool filter(sp<T>));
    template<typename T>
    vector<int> getAllMessageInterfaceOutIds();
    template<typename T>
    vector<int> getAllMessageInterfaceOutIds(bool filter(sp<T>));

    virtual bool removeMessageInterface(int interfaceKey);
    virtual bool removeMessageInterface(unordered_map<int, sp<MessageInterface>*>::iterator interfaceIt);

    string messagingUnitKey = "BasicMessagingUnit";
protected:

    virtual void tick() override;

    multimap<int, sp<Frame>> frames;
    unordered_set<sp<Frame>> addedFrames;
    vector<sp<UpdatingFrame>> alwaysUptadingFrames;  
    void removeFrameFromArrays(multimap<int, sp<Frame>>::iterator itr);  

    vector<pair<int, sp<Message>>> messagesToProcess;

    int maxMessageLifetime = 1;

    int addMessagingUnit(sp<MessageInterface> interface_in, sp<MessageInterface> interface_out);
    int lastMsgInterfaceOutId = 0;
    // unordered_map<int, weak_ptr<MessageInterface>> msgInterfaces_out;
    sp<MessageInterface> msgInterfaceExtern;
    // paires : id -> sp<MessageInterface>[2] = {in, out}
    unordered_map<int, sp<MessageInterface>*> msgInterfaces;

    void retreiveMessages();
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
    vector<sp<Frame>> ret;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end() && ret.size() < n; it++){
        if(dynamic_pointer_cast<T>(it->second)) {
            ret.push_back(it->second);
            removeFrameFromArrays(it);
            it--;
        }
    }

    while(ret.size() < n)
        ret.push_back(nullptr);

    return ret;
}

template<typename T>
vector<sp<Frame>> MessagingUnit::popAllFrames(){
    vector<sp<Frame>> ret;

    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++){
        if(dynamic_pointer_cast<T>(it->second)) {
            // delIterators.push_back(it);
            ret.push_back(it->second);
            removeFrameFromArrays(it);
            it--;
        }
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

#ifdef END_MESSAGEINTERFACE_H

template<typename T>
int MessagingUnit::getMessageInterfaceOutId(){
    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++){
        if(it->second[1] && dynamic_cast<T *>(it->second[1]->destination))
            return it->first;
    }

    return -1;
}

template<typename T>
int MessagingUnit::getMessageInterfaceOutId(bool filter(sp<T>)){
    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++){
        if(it->second[1]){
            sp<T> T_cast = dynamic_cast<T *>(it->second[1]->destination);
            if(T_cast && filter(T_cast))
                return it->first;
        }
    }

    return -1;
}

template<typename T>
vector<int> MessagingUnit::getAllMessageInterfaceOutIds(){
    vector<int> ret;

    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++){
        if(it->second[1] && dynamic_cast<T *>(it->second[1]->destination))
            ret.push_back(it->first);
    }

    return ret;
}

template<typename T>
vector<int> MessagingUnit::getAllMessageInterfaceOutIds(bool filter(sp<T>)){
    vector<int> ret;

    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++){
        if(it->second[1]){
            sp<T> T_cast = dynamic_cast<T *>(it->second[1]->destination);
            if(T_cast && filter(T_cast))
                ret.push_back(it->first);
        }
    }

    return ret;
}

#endif

#endif