#include "std_bot/dataContainers/DataBuffer.h"
#include "std_bot/dataContainers/MessageDataBuffer.h"
#include "std_bot/messaging/DofusNetworkMessage.h"
#include "std_bot/messaging/MessageInterface.h"
#include "std_bot/test_class/RawMessage.h"
#include "std_bot/logger/Logger.h"
#include "std_bot/connection/NetworkConnection.h"
#include "ReadFrame.h"
#include "ptr_include.h"
#include "thread_include.h"
#include "time_include.h"
#include "socket_include.h"

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

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
        m->serialize(out);
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

    int sock;
    sockaddr_in addr_in;
    int addrlen = sizeof(addr_in);
    int opt = 1;

    if(sock = socket(AF_INET, SOCK_STREAM, 0)){
        cout << "Socket contruit" << endl;
    } else {
        cout << "Erreur de construction du socket" << endl;
        return -1;
    }

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &addr_in.sin_addr)<=0){
        cout << "Adresse invalide" << endl;
        return -1;
    }

    if(connect(sock, (sockaddr *) &addr_in, sizeof(sockaddr)) < 0){
        cout << "Impossible de se connecter" << endl;
        return -1;
    } else {
        cout << "Le client est connecté" << endl;
    }

    NetworkConnection conn;
    conn.connected = true;
    conn.connectedAddress = "localhost";
    conn.socket = sock;

    sp<MessageDataBuffer> data(nullptr);
    while(!data){
        data = conn.readData(22);
    }

    cout << data->toCharArray() << endl;

    string s_send = "Salutttttt !";
    sp<MessageDataBuffer> mdb_send(new MessageDataBuffer((char *) s_send.c_str(), s_send.size()));
    mdb_send->cursor_reset();

    sleep(1);

    if(!conn.writeData(mdb_send)){
        Logger::write("Erreur d'envoi du msg", LOG_IMPORTANT_WARNING); 
        return -1;
    }

    Logger::endInstance();
    return 0;
}

/*
TODO:

Rendre les classes abstraites totalement abstraites => fonctions purement virutelles
Commentaires
Documentation? => d'abord une implémentation

*/
