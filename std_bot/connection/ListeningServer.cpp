#include "ListeningServer.h"

sp<thread> ListeningServer::beginListening(){
    int opt = 1;

    if(!(listeningSocket = socket(AF_INET, SOCK_STREAM, 0))){
        Logger::write("Cannot build the listening socket.", LOG_ERROR);
        return nullptr;
    }

    if(setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        Logger::write("Cannot configure the listening socket.", LOG_ERROR);
        return nullptr;
    }

    listeningAddr.sin_family = AF_INET;
    listeningAddr.sin_addr.s_addr = INADDR_ANY;
    listeningAddr.sin_port = htons(listeningPort);

    if(bind(listeningSocket, (sockaddr *) &listeningAddr, sizeof(listeningAddr)) < 0) {
        Logger::write("Cannot bind adress and listening socket.", LOG_ERROR);
        return nullptr;
    }

    if(listen(listeningSocket, 0) < 0){
        Logger::write("Cannot listen to the socket.", LOG_ERROR);
        return nullptr;
    }

    run = true;
    listeningThread = make_shared<thread>(&ListeningServer::listenLoop, this);
    return listeningThread;
}

void ListeningServer::listenLoop(){
    fd_set fileDescriptor;
    FD_ZERO(&fileDescriptor);

    //1 sec timeout for the select function.
    timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    int addrlen = sizeof(listeningAddr);

    while (run) {    
        unique_lock<timed_mutex> connectionsLock(connectionsMutex, defer_lock);
        if(connectionsLock.try_lock_for(chrono::milliseconds(10))){
            for (int i = 0; i < orphanConnections.size(); i++){
                if(!orphanConnections[i])
                    orphanConnections.erase(orphanConnections.begin() + i);
            }
            for (int i = 0; i < connections.size(); i++){
                if(connections[i].expired())
                    connections.erase(connections.begin() + i);
            }

            connectionsLock.unlock();
        }
        // if(lastConnection.expired()){
        //     lastConnection = connections.back();
        // }

        FD_SET(listeningSocket, &fileDescriptor);

        int sel = pselect(listeningSocket + 1, &fileDescriptor, NULL, NULL, &timeout, NULL);
        if(sel < 0){
            Logger::write("Error on listening socket. Terminating the listening loop.", LOG_ERROR);
            run = false;

        } else if (sel > 0){
            int newSocket = accept(listeningSocket, (sockaddr *) &listeningAddr, (socklen_t *) &addrlen);

            if(newSocket < 0){
                Logger::write("Cannot accept incoming connection.", LOG_ERROR);

            } else {
                if(connections.size() >= maxConnections) {
                    Logger::write("Connection refused : too many connections are already beeing handled.", LOG_INFO);
                    refuseConnection(newSocket);
                } else {
                    sp<ServerConnection> newConnection = acceptConnection(newSocket);
                    if(newConnection){
                        if(connectionsLock.try_lock_for(chrono::seconds(1))){
                            orphanConnections.push_back(newConnection);
                            // lastConnection = newConnection;
                            newConnection->onConnection();
                            connectionsLock.unlock();
                        } else {
                            Logger::write("Could not accept a connection because the connections' vector was blocked for too long. There might be a deadlock!", LOG_ERROR);
                            refuseConnection(newSocket);
                            continue;
                        }
                    }
                }
            }
        }
    }
}

//TODO : supprimer cette fonction et laisser les classes enfant les implémenter
sp<ServerConnection> ListeningServer::acceptConnection(int serverSocket){
    // sp<BasicPrefixConnection> newConnection( new BasicPrefixConnection());

    // //TODO : mettre la vraie addresse
    // newConnection->connectedAddress = "localhost";
    // newConnection->connected = true;
    // newConnection->sock = serverSocket;

    // return newConnection;
    return nullptr;
}

void ListeningServer::refuseConnection(int serverSocket){
    close(serverSocket);
}

sp<ServerConnection> ListeningServer::getLastConnection(){
    unique_lock<timed_mutex> connectionsLock(connectionsMutex, defer_lock);
    if(connectionsLock.try_lock_for(chrono::milliseconds(100))){
        if(orphanConnections.size() <= 0)
            return nullptr;
        sp<ServerConnection> connection = orphanConnections.back();
        orphanConnections.pop_back();
        connections.push_back(connection);

        return connection;
    } else {
        Logger::write("Could not access the connections' vector. Careful of deadlocks.", LOG_WARNING);
        return nullptr;
    }
}

template<typename T>
sp<T> ListeningServer::getConnection(){
    unique_lock<timed_mutex> connectionsLock(connectionsMutex, defer_lock);
    if(connectionsLock.try_lock_for(chrono::milliseconds(10))) {
        for(sp<ServerConnection> oc : orphanConnections){
            if(dynamic_pointer_cast<T>(oc))
                return oc;
        }

        for(weak_ptr<ServerConnection> c : connections){
            if(dynamic_pointer_cast<T>(c))
                return c;
        }

    } else {
        Logger::write("Could not access the connections' vector. Careful of deadlocks.", LOG_WARNING);
        return nullptr;       
    }
}