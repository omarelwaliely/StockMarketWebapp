#include "login.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../response.h"
#include <jwt-cpp/jwt.h>

Response Login::execute(const Request& request) {
    std::ifstream file("/home/omar/Documents/GitHub/StockMarketWebapp/Server/database/users.json");
    nlohmann::json users;
    if (!file.is_open()) {
        users = {}; // File doesn't exist, create an empty JSON object
    } else {
        file >> users;
        file.close();
    }
    try {
        nlohmann::json bodyJson = nlohmann::json::parse(request.getBody());
        std::string email = bodyJson["email"];
        std::string password = bodyJson["password"];
        if(email== "" || password == ""){
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "Fill all fields."}
            };
            Response res(body.dump(),"encryptedsecretkey" , 300);
            return res; 
        }
        else if (users.find(email) != users.end()) {
            if(users[email]["password"]!=password){ //check if password matches
                std::cout << "Error: Password is incorrect. " << std::endl;
                nlohmann::json body = {
                {"status", "failed"},
                {"message", "password is incorrect."}
                };
                Response res(body.dump(),"encryptedsecretkey", 500);
                return res;
            }
            nlohmann::json payload = {
            {"email", email}
            };
            auto token = jwt::create().set_type("JWS")
                                    .set_issuer("auth0")
                                    .set_payload_claim("custom", jwt::claim(payload.dump()))
                                    .sign(jwt::algorithm::hs256{"secret"});//hidden_secret should probably be kept in an env var ill leave it for now
            std::string tokenString = token;
            nlohmann::json body = {
            {"status", "Success"},
            {"token", tokenString}
            };
            // nlohmann::json body = {
            // {"status", "Success"}
            // };

            
            Response res( body.dump(),"encryptedsecretkey",200);  //we have a successful response
            return res; 
        }
        else{
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "This account does not exist."}
            };
            Response res(body.dump(),"encryptedsecretkey" , 300);
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
