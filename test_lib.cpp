#include "std_bot/dataContainers/DataBuffer.h"
#include "std_bot/dataContainers/MessageDataBuffer.h"
#include "std_bot/messaging/MessageInterface.h"
#include "std_bot/test_class/RawMessage.h"
#include "std_bot/logger/Logger.h"
#include "std_bot/connection/NetworkConnection.h"
#include "ReadFrame.h"
#include "ptr_include.h"
#include "thread_include.h"
#include "time_include.h"
#include "socket_include.h"
#include "std_bot/connection/PrefixNetworkConnection.h"
#include "BasicPrefixConnection.h"
#include "BasicPrefixedMessage.h"
#include "ListeningServer.h"
#include "WorkingUnit.h"
#include "MessagingUnit.h"
#include "OtherUselessFrame.h"
#include "ConnectionUnit.h"
#include "BegConnectionMessage.h"
#include "ConnectionManagerFrame.h"
#include "ConnectionRequestMessage.h"
#include "GetConnectionFocusRequestMessage.h"
#include "SendPacketRequestMessage.h"

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void send1(sp<MessageInterface> i){
    sp<RawMessage> rm = make_shared<RawMessage>();
    sp<MessageDataBuffer> mdb = make_shared<MessageDataBuffer>();
    mdb->writeUTF("Fonction 1");
    mdb->cursor_reset();

    rm->deserialize(mdb);

    i->send(rm);
}

void send2(sp<MessageInterface> i){
    sp<RawMessage> rm = make_shared<RawMessage>();
    sp<MessageDataBuffer> mdb = make_shared<MessageDataBuffer>();
    mdb->writeUTF("Fonction 2");
    mdb->cursor_reset();

    rm->deserialize(mdb);

    i->send(rm);
}

int main() {
    Logger::beginInstance();
    // l->init();

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

    vector<sp<Message>> n_messages;
    

    for(sp<Message> m : messages){
        if(dynamic_pointer_cast<RawMessage>(m)){
            sp<RawMessage> msg = dynamic_pointer_cast<RawMessage>(m);
            n_messages.push_back(msg);
            // sp<MessageDataBuffer> out = make_shared<MessageDataBuffer>();
            // dynamic_pointer_cast<RawMessage>(m)->serialize(out);
            // out->cursor_reset();
            // cout << out->readUTF() << endl;
        }
    }

    for(sp<Message> m: messages){
        sp<MessageDataBuffer> out = make_shared<MessageDataBuffer>();
        dynamic_pointer_cast<NetworkMessage>(m)->serialize(out);
        out->cursor_reset();
        cout << out->readUTF() << endl;
    }

    sp<Message> null_m(nullptr);
    cout << (null_m == nullptr) << endl;
    if(!messages[0])
        cout << "nullptr" << endl;
    else
        cout << "not nullptr" << endl;

    // ReadFrame frame;

    // sp<Message> ms(new Message());

    // cout << ms->getId() << endl;
    // cout << RawMessage::protocolId << endl;
    // cout << messages[0]->getId() << endl;

    MessageDataBuffer mdb = MessageDataBuffer({1, 2, 3});
    MessageDataBuffer mdb2;
    mdb2 = mdb;

    // int sock;
    // sockaddr_in addr_in;
    // int addrlen = sizeof(addr_in);
    // int opt = 1;

    // if(sock = socket(AF_INET, SOCK_STREAM, 0)){
    //     cout << "Socket contruit" << endl;
    // } else {
    //     cout << "Erreur de construction du socket" << endl;
    //     return -1;
    // }

    // addr_in.sin_family = AF_INET;
    // addr_in.sin_port = htons(8080);

    // if(inet_pton(AF_INET, "127.0.0.1", &addr_in.sin_addr)<=0){
    //     cout << "Adresse invalide" << endl;
    //     return -1;
    // }

    // if(connect(sock, (sockaddr *) &addr_in, sizeof(sockaddr)) < 0){
    //     cout << "Impossible de se connecter" << endl;
    //     return -1;
    // } else {
    //     cout << "Le client est connecté" << endl;
    // }

    // conn.connected = true;
    // conn.connectedAddress = "localhost";
    // conn.socket = sock;
    // conn.connectTo("127.0.0.1", 8080);

    sp<MessagingUnit> msgUnit1(new MessagingUnit());
    sp<MessagingUnit> msgUnit2(new ConnectionUnit());
    sp<MessagingUnit> msgUnit3(new MessagingUnit());

    // TODO : test avec FromConnectionMessage -> vérifier qu'il est bien initialisé qd il faut
    // Faire des tests avec MessageInterface::destination et sécuriser un peu tout ca
    // Dans MessagingUnit::~MessagingUnit() -> vérifier que l'on est bien la destination de msgInterface_in avant de mettre à nullptr
    MessagingUnit::link(msgUnit1, msgUnit2);
    MessagingUnit::link(msgUnit3, msgUnit2);

    sp<Frame> fr1(new ReadFrame());
    sp<Frame> fr2(new ReadFrame());
    // sp<Frame> fr3(new ReadFrame());
    sp<OtherUselessFrame> ufr2(new OtherUselessFrame());
    sp<ConnectionManagerFrame> cmfr(new ConnectionManagerFrame());
    msgUnit1->addFrame(fr1);
    msgUnit2->initFrames();
    msgUnit3->addFrame(ufr2);
    // msgUnit1->addFrame(ufr2);

    // msgUnit2->addFrame(fr2);
    // msgUnit2->addFrame(fr3);
    // msgUnit2->addFrame(fr3);
    // msgUnit2->addFrame(ufr2);
    // msgUnit2->addFrame(cmfr);

    // cout << fr1.use_count() << endl;

    msgUnit1->launch().get();
    msgUnit2->launch().get();
    msgUnit3->launch().get();

    // msgUnit1->stop();
    // msgUnit1->waitThreadEnd();
    // msgUnit1.reset();

    // sp<Message> msg(new RawMessage());
    // msgUnit1->sendSelfMessage(msg);

    // msgUnit1->sendMessage(make_shared<ConnectionRequestMessage>([](){return make_shared<BasicPrefixConnection>();}, "127.0.0.1", 8080), 0);
    msgUnit1->sendMessage(make_shared<ConnectionRequestMessage>(make_shared<BasicPrefixConnection>(), "127.0.0.1", 8080), 0);
    usleep(1000);
    msgUnit1->sendMessage(make_shared<ConnectionIdGetMessage>([](sp<Connection> c){return (bool) dynamic_pointer_cast<BasicPrefixConnection>(c);}), 0);
    usleep(1000);
    msgUnit3->sendMessage(make_shared<GetConnectionFocusRequestMessage>([](sp<Connection> c){return (bool) dynamic_pointer_cast<BasicPrefixConnection>(c);}), 0);
    // msgUnit3->sendMessage(make_shared<GetConnectionFocusRequestMessage>((vector<int>) {0}), 0);

    // new GetConnectionFocusRequestMessage((vector<int>) {0});

    // getchar();

    usleep(1500 * 1000);
    msgUnit1->sendMessage(make_shared<DisconnectRequestMessage>([](sp<Connection> c){return (bool) dynamic_pointer_cast<ClientConnection>(c);}), 0);
    cout << "disconnect" << endl;
    usleep(1500 * 1000);


    msgUnit1->stop();
    msgUnit2->stop();
    msgUnit3->stop();

    // msgUnit2->removeMessageInterface(0);
    msgUnit1->waitThreadEnd();
    msgUnit2->waitThreadEnd();
    msgUnit3->waitThreadEnd();

    cout << "tout fermé" << endl;


    // cout << "fr1 : " << fr1 << endl;
    // cout << "fr2 : " << fr2 << endl;
    // cout << "fr3 : " << fr3 << endl;
    // cout << "ufr2 : " << ufr2 << endl;

    // cout << msgUnit1->popFrame<ReadFrame>() << endl;
    // cout << msgUnit1->getFrame<OtherUselessFrame>() << endl;
    // vector<sp<Frame>> frs = msgUnit2->getFrames<OtherUselessFrame>(2);
    // for(sp<Frame> f : frs)
    //     cout << f << endl;
    // frs = msgUnit2->popAllFrames<ReadFrame>();
    // for(sp<Frame> f : frs)
    //     cout << f << endl;


    chrono::seconds s(1);
    chrono::system_clock::duration d = chrono::duration_cast<chrono::nanoseconds>(s);

    // conn.maxTimeBetweenPendingUpdates = &d;

    // WorkingUnit work;
    // work.launch();

    // usleep(500*1000);
    // cout << "lsqkqjdqlskd" << endl;

    // ListeningServer ls(8080, 2);


    // ls.beginListening();
    // bool run = true;
    // vector<sp<ServerConnection>> vsc;
    
    // while(true){
    //     sp<ServerConnection> last_co = ls.getLastConnection();
    //     if(last_co){
    //         vsc.push_back(last_co);
    //     }

    //     for(int i = 0; i < vsc.size(); i++){
    //         if (!vsc[i] || !vsc[i]->connected){
    //             vsc.erase(vsc.begin() + i);
    //             continue;
    //         }

    //         sp<BasicPrefixedMessage> msg = dynamic_pointer_cast<BasicPrefixedMessage>(vsc[i]->readMessage());
    //         if(msg)
    //             cout << msg->getMessage() << endl;
    //     }
    // }

    // while(ls.connections.size() <= 0){
    //     cout << "J'attends" << endl;
    //     usleep(500 * 1000);
    // }

    // sp<BasicPrefixConnection> conn = dynamic_pointer_cast<BasicPrefixConnection>(ls.connections.back());
    // if(!conn){
    //     cout << "Connection non valide" << endl;
    // }

    // sp<BasicPrefixedMessage> msg;

    // while(!msg){
    //     msg = dynamic_pointer_cast<BasicPrefixedMessage>(conn->readMessage());
    // }

    // if(msg) {
    //     cout << msg->getMessage() << endl;
    // }
    // sp<MessageDataBuffer> data(nullptr);
    // while(!data){
    //     data = conn.readData(22);
    // }

    // cout << data->toCharArray() << endl;

    // string s_send = "Salutttttt !";
    // sp<MessageDataBuffer> mdb_send(new MessageDataBuffer((char *) s_send.c_str(), s_send.size()));
    // mdb_send->cursor_reset();

    // sleep(1);

    // if(!conn.writeData(mdb_send)){
    //     Logger::write("Erreur d'envoi du msg", LOG_IMPORTANT_WARNING); 
    //     return -1;
    // }


    MessageDataBuffer dat({1, 2, 3});
    cout << dat.toString() << endl;
    dat.insertDataSpace(3);
    dat.write({4, 5, 6, 7});
    dat.cursor_end();
    dat.insertDataSpace(3);
    cout << dat.toString() << endl;

    // ls.stop();
    Logger::endInstance();
    return 0;
}

/*
Transmettre le "TODO tout à la fin" à l'implémentation

TODO tout à la fin :
Logger => log la ligne de l'erreur et le fichier (voir macros __LINE__ et __FILE__)
Si le besoin se ressent, mettre plus de messages d'erreur dans le logger (can't find connection/Frame/MU - not the right type of parent/connection/message - fails)
Si le besoin, mettre des try-catch en plus pour éviter les crash
enlever les assert(false) qui on servi au débug
documentation
*/
