#include "RawMessage.h"

bool RawMessage::serialize(sp<MessageDataBuffer> output){
    data.cursor_reset();

    while (!data.isExhausted()){
        output->write(data.read());
    }

    return true;
}

bool RawMessage::deserialize(sp<MessageDataBuffer> input){
    data.cursor_reset();
    int length = input->remainingDataSize();
    
    for(int i = 0; i < length; i++){
        data.write(input->read());
    }

    return true;
}