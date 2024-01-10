#include <sys/types.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#pragma once
class Server
{
private:
    int listeningSocket;
    sockaddr_in hint;

public:
    Server(int);
    Server(int, std::string);
    ~Server();
    int startListen();
};