#include <iostream>
#include "server.cpp"

int main()
{
    Server myServer(54000);
    myServer.startListen();
    return 0;
}