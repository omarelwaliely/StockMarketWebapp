#include <iostream>
#include <sys/types.h>
#include <uistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0); // explain this in comment
    if (listening == -1)                             // failure when -1
    {
        std::cerr << "Failed to create socket.";
        return -1
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // htons will switch to little or big endian if required, nhtos
    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // bind the socket 'listening' to the 'hint' data structure, we cast from sockaddr_in * to sockaddr * to fit the function parameter
    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) // error when -1
    {
        std::cerr << "Unable to bind to IP or Port";
        return -2;
    }

    // here we can start listening with maximum connections SOMAXCONN
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "unable to listen";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
    if (clientSocket == -1)
    {
        std::cerr << "client unable to connect";
        return -4;
    }
    close(listening);

    // clean up the buffers
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    // obtain clients host info and print it
    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result)
    {
        cout << host << "connected on " << svc << endl;
    }
    else // it couldnt be obtained so we do it manually
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);            // set the address of the client but first convert to little / big endian if needed
        cout << host << "connected on " << ntohs(client.sin_port) << endl; // we use ntohs to again convert from little endian to big if we needed
    }

    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            std::cerr << "connection issue detected." << endl;
            break;
        }
        else if (bytesRecv == 0)
        {
            cout << "Client disconnected" << endl;
            break;
        }
        cout << "Received: " << string(buf, 0, bytesRecv) << endl;
        send(clientSocket, buf, bytesRecv + 1, 0); // adding 1 due to the 0 at the end, this just echos
    }
}