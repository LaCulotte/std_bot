#include "ReadFrame.h"

ReadFrame::ReadFrame(){

}

ReadFrame::~ReadFrame(){
    
}

bool ReadFrame::update(){
    cout << "update" << endl;

    return true;
}

bool ReadFrame::computeMessage(sp<Message> message){
    bool validMessage = false;
    
    if(message->getId() == RawMessage::protocolId){
        validMessage = true;
    
        cout << "salut" << endl;
    } 

    if(validMessage || alwaysUpdate)
        update();

    return validMessage;
}

