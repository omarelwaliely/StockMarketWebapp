#include "aquireStocks.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../response.h"
#include <jwt-cpp/jwt.h>

Response AquireStocks::execute(const Request& request) {
    try {
        //validate token
        nlohmann::json bodyJson = nlohmann::json::parse(request.getBody());
        nlohmann::json headJson = nlohmann::json::parse(request.getHeader());
        if (!(headJson.find("token") != headJson.end() &&
            headJson["token"].is_string() &&
            !headJson["token"].is_null())) {
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "Invalid or missing 'token' field in the request header"}
            };
            Response res(body.dump(), "encryptedsecretkey", 400);
            return res;
        }
        else{
            nlohmann::json body = {
            {"status", "Success"},
            {"service", true},
            {"endpoint","/fetchstocks"}
            };
            Response res( body.dump(),"encryptedsecretkey",200);  //we have a successful response
            return res; 
        }
    } catch (const nlohmann::json::exception& e) { //if there is an  exception with reading the json we will let that error be known in the json
        std::cout << "Error parsing JSON: " << e.what() << std::endl; //we print the error in console
        nlohmann::json body = {
            {"status", "failed"},
            {"message", "Invalid JSON format in the request body during aquirestock"}
        };
        Response res(body.dump(),"encryptedsecretkey", 300);
        return res;
    }
}
