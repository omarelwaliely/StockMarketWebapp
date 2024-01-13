#include "register.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../response.h"

Response Register::execute(const Request& request) {
    std::ifstream file("./database/users.json");
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
        std::string firstName = bodyJson["firstName"];
        std::string lastName = bodyJson["lastName"];
        if(email== "" || password == "" || firstName == "" || lastName == ""){
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "Fill all fields."}
            };
            Response res(body.dump(),"encryptedsecretkey" , 300);
            return res; 
        }
        if (users.find(email) != users.end()) { //theres a collision the email exists, since its the primary key we cant allow that
            std::cout << "Error: User with this email already exists." << std::endl;
            nlohmann::json body = {
                {"status", "failed"},
                {"message", "User with this email already exists."}
            };
            Response res(body.dump(),"encryptedsecretkey", 500);
            return res;
        }

        users[email] = {
            {"email", email},
            {"password", password},
            {"firstName", firstName},
            {"lastName", lastName},
            {"money",2000}
        };
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
        std::ofstream outFile("./database/users.json");
        outFile << users.dump(4); //put it in the file
        outFile.close();
        std::cout << "User registered" << std::endl; //just in case for debugging
        Response res( body.dump(),"encryptedsecretkey",200);  //we have a successful response
        return res; 
            
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
