#include <iostream>
#include "endpointStrategy.h"
#include "../request.h"
#include "../response.h"
#include "login.cpp"
#include "logout.cpp"
#include "register.cpp"
#include "notFound.cpp"
#include "changeStock.cpp"
#include "processedChange.cpp"

#pragma once
class EndpointHandler {
private:
    EndpointStrategy* strategy;

public:
    EndpointHandler() : strategy(nullptr) {}

    Response handleRequest(const Request& request) {
        std::string endpoint = request.getEndpoint();
        if (endpoint == "/login") {
            strategy = new Login();
        }
        else if (endpoint == "/logout") {
            strategy = new Logout();
        }
        else if (endpoint == "/register") {
            strategy = new Register();
        }
        else if (endpoint == "/changestock") {
            strategy = new ChangeStock();
        }
        else if (endpoint == "/processedchange") {
            strategy = new ProcessedChange();
        }
        else{
            std::cout << "no such endpoint exists!";
            strategy = new NotFound();
        }
        Response res = strategy->execute(request);
        delete strategy;
        strategy = nullptr;
        return res;
    }
};