#include "MessagingUnit.h"

MessagingUnit::MessagingUnit(){
    msgInterface_in = make_shared<MessageInterface>(); 
}

MessagingUnit::~MessagingUnit(){
    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++)
        it->second->parent = nullptr;
}

bool MessagingUnit::addFrame(sp<Frame> frame, bool alwaysUpdates){ 
    if(addedFrames.find(frame) != addedFrames.end())
        return false;

    addedFrames.insert(frame);

    frames.insert(pair<int, sp<Frame>>(frame->getPriority(), frame));
    if(alwaysUpdates){
        sp<UpdatingFrame> upFrame = dynamic_pointer_cast<UpdatingFrame>(frame);
        if(upFrame)
            alwaysUptadingFrames.push_back(upFrame);
    }

    frame->parent = this;

    return true;
}

void MessagingUnit::removeFrameFromArrays(multimap<int, sp<Frame>>::iterator itr){
    sp<Frame> frame = itr->second;
    frames.erase(itr);

    if(frame && dynamic_pointer_cast<UpdatingFrame>(frame)){
        for(int i = 0; i < alwaysUptadingFrames.size(); i++)
            if(alwaysUptadingFrames[i] == frame) {
                alwaysUptadingFrames.erase(alwaysUptadingFrames.begin() + i);
                break;
            }
    }

    auto it = addedFrames.find(frame);
    if(it != addedFrames.end())
        addedFrames.erase(it);
}

sp<Frame> MessagingUnit::removeFrame(sp<Frame> frame) {
    for(multimap<int, sp<Frame>>::iterator it = frames.lower_bound(frame->getPriority()); it != frames.upper_bound(frame->getPriority()); it++){
        if(it->second == frame) {
            removeFrameFromArrays(it);
            return it->second;
        }
    }

    return nullptr;
}

sp<Frame> MessagingUnit::removeFrame(Frame * frame) {
    for(multimap<int, sp<Frame>>::iterator it = frames.lower_bound(frame->getPriority()); it != frames.upper_bound(frame->getPriority()); it++){
        if(it->second.get() == frame) {
            removeFrameFromArrays(it);
            return it->second;
        }
    }

    return nullptr;   
}

vector<sp<Frame>> MessagingUnit::removeFrames(vector<sp<Frame>> frames, bool with_nullptr){
    vector<sp<Frame>> retFrames;
    for(sp<Frame> frame : frames){
        if(with_nullptr) {
            retFrames.push_back(removeFrame(frame));
        } else {
            sp<Frame> removedFrame = removeFrame(frame);
            if(removedFrame)
                retFrames.push_back(removedFrame);
        }
    }

    return retFrames;
}

vector<Frame *> MessagingUnit::removeFrames(vector<Frame *> frames, bool with_nullptr){
    vector<Frame *> retFrames;
    for(Frame * frame : frames){
        if(with_nullptr) {
            retFrames.push_back(removeFrame(frame).get());
        } else {
            sp<Frame> removedFrame = removeFrame(frame);
            if(removedFrame)
                retFrames.push_back(removedFrame.get());
        }
    }

    return retFrames;
}

bool MessagingUnit::canAddMessagingUnit(sp<MessagingUnit> otherMU){
    return (msgInterfaces_out.find(otherMU->messagingUnitKey) == msgInterfaces_out.end());
}

bool MessagingUnit::addMessagingUnit(sp<MessagingUnit> otherMU){
    int old_size = msgInterfaces_out.size();
    msgInterfaces_out.insert(pair<string, sp<MessageInterface>>(otherMU->messagingUnitKey, otherMU->msgInterface_in));

    return old_size != msgInterfaces_out.size();
}

bool MessagingUnit::link(sp<MessagingUnit> mu1, sp<MessagingUnit> mu2){
    if(mu1->canAddMessagingUnit(mu2) && mu2->canAddMessagingUnit(mu1)){
        mu1->addMessagingUnit(mu2);
        mu2->addMessagingUnit(mu1);

        return true;
    }

    return false;
}

bool MessagingUnit::sendSelfMessage(sp<Message> message){
    msgInterface_in->send(message);
    return true;
}

bool MessagingUnit::sendMessage(sp<Message> message, string destKey){
    if(msgInterfaces_out.find(destKey) != msgInterfaces_out.end()){
        weak_ptr<MessageInterface> w_out = msgInterfaces_out.at(destKey);

        if(!w_out.expired()){
            sp<MessageInterface> out = w_out.lock();

            out->send(message);
            return true;
        }
    }

    return false;
}

void MessagingUnit::retreiveMessages(){
    vector<sp<Message>> messages = msgInterface_in->receive();

    messagesToProcess.insert(messagesToProcess.end(), messages.begin(), messages.end());    
}

void MessagingUnit::tick(){
    retreiveMessages();

    unordered_set<sp<UpdatingFrame>> toUpdateFrames;

    for(int i = 0; i < messagesToProcess.size(); i++){
        sp<Message> message = messagesToProcess[i];
        message->keepInLoop = false;
        for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++){
            if(it->second->computeMessage(message)) {
                if(dynamic_pointer_cast<UpdatingFrame>(it->second))
                    toUpdateFrames.insert(dynamic_pointer_cast<UpdatingFrame>(it->second));
                if(!message->keepInLoop)
                    break;
                message->keepInLoop = false;
            }
        }            

        if(++message->lifetime >= maxMessageLifetime){
            messagesToProcess.erase(messagesToProcess.begin() + i);
            i--;
        }
    }

    for(sp<UpdatingFrame> upFrame : alwaysUptadingFrames)
        toUpdateFrames.insert(upFrame);

    for(unordered_set<sp<UpdatingFrame>>::iterator it = toUpdateFrames.begin(); it != toUpdateFrames.end(); it++)
        (*it)->update(); 
}