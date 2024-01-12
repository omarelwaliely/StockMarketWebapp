#include <iostream>
#include "server.cpp"

int main()
{
    Server myServer(54002,54003);
    myServer.startListen();
    return 0;
}