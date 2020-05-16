#include "ClientConnection.h"

bool ClientConnection::connectTo(string address, int port){
    sockaddr_in addr_in;
    int addr_len = sizeof(addr_in);

    if(!(sock = socket(AF_INET, SOCK_STREAM, 0))){
        Logger::write("Cannot create socket.", LOG_ERROR);
        return false;
    }

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);

    if(inet_pton(AF_INET, address.c_str(), &addr_in.sin_addr) <= 0){
        Logger::write(address + " is not a valid IP adress. Aptenting a dns request.", LOG_INFO);
        
        vector<string> ips = dns_lookup_ipv4(address);

        bool correctAddress = false;
        for(int i = 0; i < ips.size() && !correctAddress; i++){
            if(inet_pton(AF_INET, address.c_str(), &addr_in.sin_addr) > 0)
                correctAddress = true;
        }

        if(!correctAddress){
            Logger::write("Impossible to find " + address + ".", LOG_WARNING);
            return false;
        }
    }

    if(connect(sock, (sockaddr *) &addr_in, sizeof(sockaddr)) < 0){
        Logger::write("Cannot connect to :" + address + ".", LOG_WARNING);
        return false;
    }

    Logger::write("Connected to : " + address + ".");
    connected = true;
    connectedAddress = address;
    return true;
}

