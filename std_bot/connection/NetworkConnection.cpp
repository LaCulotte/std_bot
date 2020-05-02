#include "NetworkConnection.h"

NetworkConnection::NetworkConnection(){
    FD_ZERO(&rfd);
}

NetworkConnection::NetworkConnection(const NetworkConnection& other){
    FD_ZERO(&rfd);
}

NetworkConnection& NetworkConnection::operator=(const NetworkConnection& other){
    FD_ZERO(&rfd);

    return *this;
}

NetworkConnection::~NetworkConnection(){
}


sp<MessageDataBuffer> NetworkConnection::readData(int length){
    sp<MessageDataBuffer> ret(nullptr);

    if (isThereMessage()) {
        char *data = (char *) malloc(length * sizeof(char));
        int recv_len = recv(socket, data, length, 0);
        if(recv_len == 0){
            Logger::write("Error while reading data from " + connectedAddress + " : connection was closed.", LOG_WARNING);
            disconnect();
            return ret;
        }
        ret = make_shared<MessageDataBuffer>(data, recv_len);
    }

    return ret;
}

bool NetworkConnection::writeData(sp<MessageDataBuffer> data){
    if(!connected)
        return false;

    int send_ret = send(socket, data->toCharArray(), data->size(), 0);

    if(send_ret == -1){
        Logger::write("Error while sending data to " + connectedAddress + ".", LOG_WARNING);
        Logger::write("TODO : get and display error id and message", LOG_INFO);
        return false;
    }

    if(send_ret < data->size()){
        Logger::write("Error while sending data to " + connectedAddress + ".", LOG_WARNING);
        Logger::write(to_string(send_ret) + " bytes sent instead of " + to_string(data->size()) + ".", LOG_WARNING); 
        return false;
    }

    return true;
}

bool NetworkConnection::isThereMessage(){
    if(!connected)
        return false;
        
    FD_SET(socket, &rfd);
    int sel_ret = select(socket + 1, &rfd, NULL, NULL, (timeval *) &selectTimeout);

    if(sel_ret < 0){
        Logger::write("Error on connection with " + connectedAddress + ". Closing connection.", LOG_IMPORTANT_WARNING);
        Logger::write("TODO : get and display error id and message", LOG_INFO);
        disconnect();
        return false;
    }

    return (sel_ret > 0);
}