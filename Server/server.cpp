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

Server::Server(int portsvc, int portclients) { // default server which uses the wildcard ip 0.0.0.0 (any)
    listeningSocketSvc = createSocket(portsvc);
    listeningSocketClients = createSocket(portclients);
    if (listeningSocketSvc == -1 || listeningSocketClients == -1) {//failure when -1
        std::cerr << "Failed to create a socket." << std::endl;
    }
    hintSvc.sin_family = AF_INET; //family is ipv4
    hintSvc.sin_port = htons(portsvc); // htons will switch to little or big endian if required

    hintClients.sin_family = AF_INET; //family is ipv4
    hintClients.sin_port = htons(portclients); // htons will switch to little or big endian if required

    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET, "0.0.0.0", &hintSvc.sin_addr);
    inet_pton(AF_INET, "0.0.0.0", &hintClients.sin_addr);
    logFile.open("server_log.txt", std::ios::app);
}

Server::Server(int portsvc, int portclients, std::string ipsvc, std::string ipclients) { // server that allows inputting ips
    listeningSocketSvc = createSocket(portsvc);
    listeningSocketClients = createSocket(portclients);
    if (listeningSocketSvc == -1 || listeningSocketClients == -1) {//failure when -1
        std::cerr << "Failed to create a socket." << std::endl;
    }
    hintSvc.sin_family = AF_INET; //family is ipv4
    hintSvc.sin_port = htons(portsvc); // htons will switch to little or big endian if required

    hintClients.sin_family = AF_INET; //family is ipv4
    hintClients.sin_port = htons(portclients); // htons will switch to little or big endian if required
    
    // below we bind the address by first converting the string to a number then we bind it to our hints address, note that 0.0.0.0 means any address
    inet_pton(AF_INET,ipsvc.c_str(), &hintSvc.sin_addr);
    inet_pton(AF_INET, ipclients.c_str(), &hintClients.sin_addr);
    logFile.open("server_log.txt", std::ios::app);
}

Server::~Server() {
    if (logFile.is_open()) {
        logFile.close();
    }
}


void Server::log(const std::string& message) {
    std::time_t now = std::time(0);
    std::tm* localTime = std::localtime(&now);
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "[%Y-%m-%d %H:%M:%S]", localTime);
    std::cout << message << std::endl<<std::endl;
    if (logFile.is_open()) {
        logFile << timeBuffer << " " << message << std::endl << std::endl;
    }
}
int Server::createSocket(int port) {
    return socket(AF_INET, SOCK_STREAM, 0); //create a socket for the ipv4 protocol no flags
}

int Server::setupSocket() {
    if (listeningSocketSvc < 0 ||listeningSocketClients < 0  ) { //there was an error in initialization
        std::cerr << "The socket failed, check previous errors." << std::endl;
        return -1;
    }
    return 0;
}

int Server::startListen() {
    EndpointHandler handler; //this is our endpoint strategy handler

    //below are the failures
    if (setupSocket() < 0) {
        return -1; 
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

    int clientSocket = acceptConnection(client, clientSize, host, svc,listeningSocketClients); 
    if (clientSocket == -1) { //failure and error printed in the accept connection function
        return -4;
    }

    close(listeningSocketClients); //we can close now
    displayClientInfo(client, host, svc); //print the info for debugging and logging
    processClientRequests(clientSocket, handler); //this is where requests and responses are managed note handleService is called here
    return 0;
}


int Server::handleService(){
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; //buffer to store host info
    char svc[NI_MAXSERV];

    int serviceSocket = acceptConnection(client, clientSize, host, svc,listeningSocketSvc); 
    if (serviceSocket == -1) { //failure and error printed in the accept connection function
        return -4;
    }

    close(listeningSocketSvc); //we can close now
    displayClientInfo(client, host, svc); //print the info for debugging and logging
    return serviceSocket;
}

int Server::bindSocket() {
    if (bind(listeningSocketSvc, (sockaddr *)&hintSvc, sizeof(hintSvc)) == -1 || bind(listeningSocketClients, (sockaddr *)&hintClients, sizeof(hintClients)) == -1 ) { //bind to to the listening socket now that the hint structures are complete
        std::cerr << "Unable to bind to IP or Port" << std::endl;
        return -1;
    }
    return 0;
}

int Server::startListening() {
    if (listen(listeningSocketSvc, SOMAXCONN) == -1 || listen(listeningSocketClients, SOMAXCONN) == -1) { //initialize the listening process
        std::cerr << "Unable to listen" << std::endl;
        return -1;
    }
    return 0;
}

int Server::acceptConnection(sockaddr_in& client, socklen_t& clientSize, char* host, char* svc, int listeningSocket) { //accept connections
    int clientSocket = accept(listeningSocket, (sockaddr *)&client, &clientSize);
    if (clientSocket == -1) {
        std::cerr << "Client unable to connect" << std::endl;
        return -1;
    }
    return clientSocket;
}

void Server::displayClientInfo(const sockaddr_in& client, char* host, char* svc) {
    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0); //get the info of the connection host
    if (result) {
        std::cout << host << " connected on " << svc << std::endl;
    } else { //if it fails we obtain it manually
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }
}
void Server::processClientRequests(int clientSocket, EndpointHandler& handler) {
    char buf[4096];
    char bufsvc[4096];
    int serviceSocket = handleService();
    if (serviceSocket <0){
        std::cerr << "error in service socket";
        return;
    }
    while (true)
    {
        memset(buf, 0, 4096);
        memset(bufsvc, 0, 4096);
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

        try
        {
            nlohmann::json jsonReq = nlohmann::json::parse(message);
            Request req(jsonReq["endpoint"], jsonReq["body"].dump(), jsonReq["header"].dump());
            Response res;
            res = handler.handleRequest(req);
            nlohmann::json responseJson = {
                {"header", res.getHeader()},
                {"body", res.getBodyJson()},
                {"code", res.getCode()}};

            std::string responseStr = responseJson.dump(); // convert back to string
            log((std::string)"RECEIVED FROM CLIENT: " + std::string(buf, 0, bytesRecv));
            nlohmann::json resBodyJson = nlohmann::json::parse(res.getBody());
            if (resBodyJson.find("service") != resBodyJson.end() && !resBodyJson["service"].is_null()){ //we entered the condition that the request obtained requires service
                log((std::string)"SENDING TO SERVICE: " + responseStr.c_str());
                send(serviceSocket, responseStr.c_str(), responseStr.size(), 0); // send response to service
                int bytesSvcRecv = recv(serviceSocket, bufsvc, 4096, 0); //wait for reply
                if (bytesSvcRecv == -1) // error code for connection issue
                {
                    std::cerr << "Connection issue detected." << std::endl;
                    break;
                }
                else if (bytesSvcRecv == 0) // code for client disconnection
                {
                    std::cout << "Service disconnected" << std::endl;
                    break;
                }
                message = std::string(bufsvc, 0, bytesSvcRecv);
                log((std::string)"RECEIVED FROM SERVICE: " + message);
                jsonReq = nlohmann::json::parse(message);
                req.setEndpoint(jsonReq["endpoint"]);
                req.setBody(jsonReq.dump());
                res = handler.handleRequest(req);
                responseJson = {
                {"header", res.getHeader()},
                {"body", res.getBodyJson()},
                {"code", res.getCode()}};
                responseStr = responseJson.dump();
            }
            log((std::string)"SENDING TO CLIENT: " + responseStr.c_str());
            send(clientSocket, responseStr.c_str(), responseStr.size(), 0); // send response
            
        }
        catch (const nlohmann::json::exception &e)
        {
            std::cerr << "Error parsing JSON in server: " << e.what() << std::endl;
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "couldnt parse JSON"}
            };
            nlohmann::json failjson = {
                {"header", "encrypted"},
                {"body", body},
                {"code", 300}};
            std::string failure  = failjson.dump();
            log((std::string)"SENDING TO CLIENT: " + failure.c_str());
            send(clientSocket, failure.c_str(), failure.size(), 0); 
        }
    }
    logFile.close();
}