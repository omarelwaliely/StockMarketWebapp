#include <iostream>
#include "server.h"

int main()
{
    Server myServer(54000);
    myServer.listen();
    return 0;
}