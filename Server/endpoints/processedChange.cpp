#include "processedChange.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../response.h"
#include <jwt-cpp/jwt.h>

Response ProcessedChange::execute(const Request& request) {
    try {
        nlohmann::json bodyJson = nlohmann::json::parse(request.getBody());
        bool status = bodyJson["status"];
        //success
        if(status){
            nlohmann::json body = {
                {"status", "successful"},
            };
            Response res(body.dump(),"encryptedsecretkey" , 200);
            return res; 
        }
        //failure
        else{
            nlohmann::json body = {
            {"status", "failed"},
            {"message", "not enough funds or stock does not exist"},
            };
            Response res( body.dump(),"encryptedsecretkey",300);  //we have a successful response
            return res; 
        }
    } catch (const nlohmann::json::exception& e) { //if there is an  exception with reading the json we will let that error be known in the json
        std::cout << "Error parsing JSON: " << e.what() << std::endl; //we print the error in console
        nlohmann::json body = {
            {"status", "failed"},
            {"message", "Invalid JSON format in the request body"}
        };
        Response res(body.dump(),"encryptedsecretkey", 300);
        return res;
    }
}
