#include <sys/types.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include "endpoints/endpointHandler.h"
#pragma once
class Server
{
private:
    int listeningSocketSvc;
    int listeningSocketClients;
    sockaddr_in hintSvc;
    sockaddr_in hintClients;
    int createSocket(int port);
    int setupSocket();
    int bindSocket();
    int startListening();
    int handleService();
    int acceptConnection(sockaddr_in& , socklen_t& , char* , char* ,int);
    void displayClientInfo(const sockaddr_in& , char* , char* );
    void processClientRequests(int, EndpointHandler&);

public:
    Server(int,int);
    Server(int,int, std::string, std::string);
    ~Server();
    int startListen();
};