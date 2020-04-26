#include "std_bot/dataContainers/DataBuffer.h"
#include "std_bot/dataContainers/MessageDataBuffer.h"
#include "std_bot/messaging/DofusNetworkMessage.h"
#include "std_bot/messaging/MessageInterface.h"
#include "std_bot/test_class/RawMessage.h"
#include "std_bot/logger/Logger.h"
#include "ReadFrame.h"
#include "ptr_include.h"
#include "thread_include.h"
#include "time_include.h"

void send1(sp<MessageInterface> i){
    sp<Message> rm = make_shared<RawMessage>();
    sp<MessageDataBuffer> mdb = make_shared<MessageDataBuffer>();
    mdb->writeUTF("Fonction 1");
    mdb->cursor_reset();

    rm->deserialize(mdb);

    i->send(rm);
}

void send2(sp<MessageInterface> i){
    sp<Message> rm = make_shared<RawMessage>();
    sp<MessageDataBuffer> mdb = make_shared<MessageDataBuffer>();
    mdb->writeUTF("Fonction 2");
    mdb->cursor_reset();

    rm->deserialize(mdb);

    i->send(rm);
}


int main() {
    Logger* l = Logger::getInstance();
    l->init();

    shared_ptr<MessageDataBuffer>ptr(new MessageDataBuffer());
    // shared_ptr<DataBuffer<char>> ptr_dat = dynamic_pointer_cast<DataBuffer<char>>(ptr);
    shared_ptr<DataBuffer<char>> ptr_dat = make_shared<DataBuffer<char>>();
    shared_ptr<MessageDataBuffer> p = dynamic_pointer_cast<MessageDataBuffer>(ptr_dat);

    // MessageDataBuffer *mdb = dynamic_cast<MessageDataBuffer *>(ptr_dat.get());

    dynamic_cast<MessageDataBuffer *>(ptr_dat.get());

    cout << "wesh" << endl;

    sp<MessageInterface> msgI = make_shared<MessageInterface>();

    thread t1 (send1, msgI);
    thread t2 (send2, msgI);

    usleep(1000 * 5);

    vector<sp<Message>> messages = msgI->receive();

    t1.join();
    t2.join();
    

    for(sp<Message> m : messages){
        if(dynamic_pointer_cast<RawMessage>(m)){
            sp<MessageDataBuffer> out = make_shared<MessageDataBuffer>();
            dynamic_pointer_cast<RawMessage>(m)->serialize(out);
            out->cursor_reset();
            cout << out->readUTF() << endl;
        }
    }


    // ReadFrame frame;

    // sp<Message> ms(new Message());

    // cout << ms->getId() << endl;
    // cout << RawMessage::protocolId << endl;
    // cout << messages[0]->getId() << endl;

    MessageDataBuffer mdb = MessageDataBuffer({1, 2, 3});
    MessageDataBuffer mdb2;
    mdb2 = mdb;



    Logger::endInstance();
    return 0;
}