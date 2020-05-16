#ifndef LISTENINGSERVER_H
#define LISTENINGSERVER_H

#include "socket_include.h"
#include "thread_include.h"
#include "ServerConnection.h"
#include "../test_class/BasicPrefixConnection.h"

class ListeningServer {
public:
    ListeningServer() {};
    ListeningServer(int port) { listeningPort = port; };
    ListeningServer(int port, int max) { listeningPort = port; maxConnections = max; };
    ListeningServer(const ListeningServer& other) = delete;

    virtual ListeningServer& operator=(const ListeningServer& other) = delete;
    virtual ~ListeningServer() = default;

    sp<thread> beginListening();
    bool stop() { run = false; listeningThread->join(); listeningThread.reset(); return true; };


    sp<ServerConnection> getLastConnection();
    template<typename T>
    sp<T> getConnection();

protected:
    //TODO : faire des fonctions qui permettent d'accéder à connections de manière synchrone
    //TODO : faire une liste orphanConnections et connections => dès qu'une connection est prise en charge, elle passe en weak_ptr dans connections
    vector<weak_ptr<ServerConnection>> connections;
    vector<sp<ServerConnection>> orphanConnections;
    timed_mutex connectionsMutex;
    // vector<sp<ServerConnection>> connections;

    virtual sp<ServerConnection> acceptConnection(int serverSocket);
    virtual void refuseConnection(int serverSocket);

    virtual void listenLoop();


    sp<thread> listeningThread;
    int listeningSocket;
    int listeningPort = 8080;
    sockaddr_in listeningAddr;

    bool run = false;
    int maxConnections = -1;
};

#endif