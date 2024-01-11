#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>
#include "server.h"
#include "json.hpp" //this is a common json parser, used from github
#include "response.h"
#include "endpoints/endpointHandler.h"

Server::Server(int port) { // default server which uses the wildcard ip 0.0.0.0 (any)
    listeningSocket = createSocket(port);
    if (listeningSocket == -1) {//failure when -1
        std::cerr << "Failed to create socket." << std::endl;
    }
    hint.sin_family = AF_INET; //family is ipv4
    hint.sin_port = htons(port); // htons will switch to little or big endian if required
    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
}

Server::Server(int port, std::string ip) {
    //same as above but we put in the port and ip we want
    listeningSocket = createSocket(port);
    if (listeningSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
    }
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
}

Server::~Server() {
}

int Server::createSocket(int port) {
    return socket(AF_INET, SOCK_STREAM, 0); //create a socket for the ipv4 protocol no flags
}

int Server::setupSocket() {
    if (listeningSocket < 0) { //there was an error in initialization
        std::cerr << "The socket failed, check previous errors." << std::endl;
        return listeningSocket;
    }
    return 0;
}

int Server::startListen() {
    EndpointHandler handler; //this is our endpoint strategy handler

    //below are the failures
    if (setupSocket() < 0) {
        return listeningSocket; 
    }

    if (bindSocket() == -1) {
        return -2;
    }

    if (startListening() == -1) {
        return -3;
    }

    //we succeeded so we continue, handle the client by accepting the connection 
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; //buffer to store host info
    char svc[NI_MAXSERV];

    int clientSocket = acceptConnection(client, clientSize, host, svc); 
    if (clientSocket == -1) { //failure and error printed in the accept connection function
        return -4;
    }

    close(listeningSocket); //we can close now
    displayClientInfo(client, host, svc); //print the info for debugging and logging
    processClientRequests(clientSocket, handler); //this is where requests and responses are managed

    return 0;
}

int Server::bindSocket() {
    if (bind(listeningSocket, (sockaddr *)&hint, sizeof(hint)) == -1) { //bind to to the listening socket now that the hint structure is complete
        std::cerr << "Unable to bind to IP or Port" << std::endl;
        return -1;
    }
    return 0;
}

int Server::startListening() {
    if (listen(listeningSocket, SOMAXCONN) == -1) { //initialize the listening process
        std::cerr << "Unable to listen" << std::endl;
        return -1;
    }
    return 0;
}

int Server::acceptConnection(sockaddr_in& client, socklen_t& clientSize, char* host, char* svc) { //accept connections from the client
    int clientSocket = accept(listeningSocket, (sockaddr *)&client, &clientSize);
    if (clientSocket == -1) {
        std::cerr << "Client unable to connect" << std::endl;
        return -1;
    }
    return clientSocket;
}

void Server::displayClientInfo(const sockaddr_in& client, char* host, char* svc) {
    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0); //get the info of the client
    if (result) {
        std::cout << host << " connected on " << svc << std::endl;
    } else { //if it fails we obtain it manually
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }
}
void Server::processClientRequests(int clientSocket, EndpointHandler& handler) {
    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0); // store the received data in buf
        if (bytesRecv == -1) // error code for connection issue
        {
            std::cerr << "Connection issue detected." << std::endl;
            break;
        }
        else if (bytesRecv == 0) // code for client disconnection
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::string message = std::string(buf, 0, bytesRecv); // store the buf in a string (expected to be a JSON object)
        std::cout << message;

        try
        {
            nlohmann::json jsonReq = nlohmann::json::parse(message);
            Request req(jsonReq["endpoint"], jsonReq["body"].dump(), jsonReq["header"]);
            Response res;
            res = handler.handleRequest(req);

            // testing
            nlohmann::json responseJson = {
                {"header", res.getHeader()},
                {"body", res.getBody()},
                {"code", res.getCode()}};

            std::string responseStr = responseJson.dump(); // convert back to string
            std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
            send(clientSocket, responseStr.c_str(), responseStr.size(), 0); // send response
        }
        catch (const nlohmann::json::exception &e)
        {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            nlohmann::json errorjson = {
                {"status", "failed"},
                {"message", "error parsing json"}
            };
            std::string message  = errorjson.dump();
            send(clientSocket, message.c_str(), message.size(), 0); // send error
        }
    }
}