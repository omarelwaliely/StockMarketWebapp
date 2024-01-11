#include <sys/types.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include "endpoints/endpointHandler.h"
#pragma once
class Server
{
private:
    int listeningSocket;
    sockaddr_in hint;
    int createSocket(int port);
    int setupSocket();
    int bindSocket();
    int startListening();
    int acceptConnection(sockaddr_in& , socklen_t& , char* , char* );
    void displayClientInfo(const sockaddr_in& , char* , char* );
    void processClientRequests(int, EndpointHandler&);

public:
    Server(int);
    Server(int, std::string);
    ~Server();
    int getListeningSocket () const {return listeningSocket;}
    int startListen();
};