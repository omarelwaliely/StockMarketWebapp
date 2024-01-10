#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "server.h"
#include "json.hpp" //this is a common json parser, used from github
#include "response.h"
#include "endpoints/endpointHandler.h"

Server::Server(int port) // default server which uses the wildcard ip 0.0.0.0 (any)
{
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0); // explain this in comment
    if (listeningSocket == -1)                         // failure when -1
    {
        std::cerr << "Failed to create socket." << std::endl;
    }
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port); // htons will switch to little or big endian if required
    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
}

Server::Server(int port, std::string ip) // allow only specific ip
{
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0); // explain this in comment
    if (listeningSocket == -1)                         // failure when -1
    {
        std::cerr << "Failed to create socket." << std::endl;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // htons will switch to little or big endian if required
    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
}

Server::~Server()
{
}

int Server::startListen()
{
    EndpointHandler handler; // this will be used to decide what to do at each endpoint
    if (listeningSocket < 0)
    {
        std::cerr << "The socket failed, check previous errors." << std::endl;
        return listeningSocket;
    }
    // bind the socket 'listening' to the 'hint' data structure, we cast from sockaddr_in * to sockaddr * to fit the function parameter
    if (bind(listeningSocket, (sockaddr *)&hint, sizeof(hint)) == -1) // error when -1
    {
        std::cerr << "Unable to bind to IP or Port" << std::endl;
        return -2;
    }

    // here we can start listening with maximum connections SOMAXCONN
    if (listen(listeningSocket, SOMAXCONN) == -1)
    {
        std::cerr << "unable to listen" << std::endl;
        return -3;
    }
    // will need to change this when i implement threading
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket = accept(listeningSocket, (sockaddr *)&client, &clientSize);
    if (clientSocket == -1)
    {
        std::cerr << "client unable to connect" << std::endl;
        return -4;
    }
    close(listeningSocket);

    // clean up the buffers
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    // obtain clients host info and print it
    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result)
    {
        std::cout << host << " connected on " << svc << std::endl;
    }
    else // it couldnt be obtained so we do it manually
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);                       // set the address of the client but first convert to little / big endian if needed
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl; // we use ntohs to again convert from little endian to big if we needed
    }
    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0); //store the recieved data in buf
        if (bytesRecv == -1) //error code for connection issue
        {
            std::cerr << "connection issue detected." << std::endl;
            break;
        }
        else if (bytesRecv == 0) //code for client disconnection
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        // std::string message = std::string(buf,0,bytesRecv); //store the buf in a string (expected to be a json object)'
        std::string message = R"({
  "email": "user@example.com",
  "password": "securepassword123",
  "username": "example_username"
})";
        Request req("/register" ,message,"tokenhere"); 
        Response res;
        res = handler.handleRequest(req);
        //testing
        nlohmann::json responseJson = { //create the json which wont praticullary have everything in the response object depending on which response
        {"header", res.getHeader()},
        {"body", res.getBody()},
        {"code", res.getCode()}};

        std::string responseStr = responseJson.dump(); //convert back to string
        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
        send(clientSocket, responseStr.c_str(), responseStr.size() + 1, 0); //send response
    }
    return 0;
}



