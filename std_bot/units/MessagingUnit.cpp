#include "MessagingUnit.h"

MessagingUnit::MessagingUnit(){
    msgInterfaceExtern = make_shared<MessageInterface>();
    msgInterfaceExtern->destination = this;
}

void MessagingUnit::initFrames() {
}

MessagingUnit::~MessagingUnit(){
    for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++)
        if(it->second->getParent() == this)
            it->second->resetParent();

    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++){
        it->second[0]->destination = nullptr;
        //Send disconnect message?
        free(it->second);
    }

    if(msgInterfaceExtern)
        msgInterfaceExtern->destination = nullptr;

}

bool MessagingUnit::addFrame(sp<Frame> frame, bool alwaysUpdates){ 
    if(!frame || addedFrames.find(frame) != addedFrames.end())
        return false;

    if(!frame->setParent(this))
        return false;

    addedFrames.insert(frame);

    frames.insert(pair<int, sp<Frame>>(frame->getPriority(), frame));
    if(alwaysUpdates){
        sp<UpdatingFrame> upFrame = dynamic_pointer_cast<UpdatingFrame>(frame);
        if(upFrame)
            alwaysUptadingFrames.push_back(upFrame);
    }


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

int MessagingUnit::addMessagingUnit(sp<MessageInterface> interface_in, sp<MessageInterface> interface_out){
    if(!interface_in || !interface_out)
        return -1;

    interface_in->destination = this;

    sp<MessageInterface> *itPair = (sp<MessageInterface>*) calloc(2, sizeof(sp<MessageInterface>));
    itPair[0] = interface_in;
    itPair[1] = interface_out;
    msgInterfaces.insert(pair<int, sp<MessageInterface>*>(lastMsgInterfaceOutId++, itPair));

    return lastMsgInterfaceOutId - 1;
}

bool MessagingUnit::removeMessageInterface(int interfaceKey){
    auto it = msgInterfaces.find(interfaceKey);
    if(it != msgInterfaces.end()) {
        removeMessageInterface(it);

        return true;
    } else {
        return false;
    }
}

bool MessagingUnit::removeMessageInterface(unordered_map<int, sp<MessageInterface>*>::iterator interfaceIt){
    if(interfaceIt->second)
        free(interfaceIt->second);
    msgInterfaces.erase(interfaceIt);

    return true;
}

bool MessagingUnit::link(sp<MessagingUnit> mu1, sp<MessagingUnit> mu2){
    if(!mu1 || !mu2)
        return false;
    
    sp<MessageInterface> in1out2 (new MessageInterface());
    sp<MessageInterface> in2out1 (new MessageInterface());

    mu1->addMessagingUnit(in1out2, in2out1);
    mu2->addMessagingUnit(in2out1, in1out2);

    return true;
}

bool MessagingUnit::sendSelfMessage(sp<Message> message){
    msgInterfaceExtern->send(message);
    return true;
    // return false;
}

bool MessagingUnit::sendMessage(sp<Message> message, int destKey){

    if(msgInterfaces.find(destKey) != msgInterfaces.end()){
        sp<MessageInterface> out = msgInterfaces.at(destKey)[1];

        if(out && out->destination) {
            out->send(message);
            return true;
        } else {
            removeMessageInterface(destKey);
        }  
    }

    return false;
}

void MessagingUnit::retreiveMessages(){

    for(auto it = msgInterfaces.begin(); it != msgInterfaces.end(); it++) {
        if(it->second[0] && it->second[1] && it->second[1]->destination){
            vector<sp<Message>> messages = it->second[0]->receive();
            for(sp<Message> m : messages)
                messagesToProcess.push_back(pair(it->first, m));

        } else {
            removeMessageInterface(it);
        }
    }

    if(msgInterfaceExtern)
        for(sp<Message> msg : msgInterfaceExtern->receive())
            messagesToProcess.push_back(pair(-1, msg));

}

void MessagingUnit::tick(){
    retreiveMessages();

    unordered_set<sp<UpdatingFrame>> toUpdateFrames;

    for(int i = 0; i < messagesToProcess.size(); i++){
        sp<Message> message = messagesToProcess[i].second;
        message->keepInLoop = false;

        for(multimap<int, sp<Frame>>::iterator it = frames.begin(); it != frames.end(); it++) {
            if(it->second->computeMessage(message, messagesToProcess[i].first)) {
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

// void MessagingUnit::cleanAllArrays() {
//     for(auto it = frames.begin(); it != frames.end(); it++){
//         if(!it->second){
//             frames.erase(it);
//             it--;

//             auto a_it = addedFrames.find(it->second);
//             if(a_it != addedFrames.end())
//                 addedFrames.erase(a_it);
//         }
//     }

//     for(int i = 0; i < alwaysUptadingFrames.size(); i++){
//         if(!alwaysUptadingFrames[i]){
//             alwaysUptadingFrames.erase(alwaysUptadingFrames.begin() + i);
//             i--;
//         }
//     }
// }