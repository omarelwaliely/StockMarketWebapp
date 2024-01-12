#include <string>
#include "json.hpp"
#pragma once
class Request {
public:
    Request(std::string endpoint, std::string body, std::string header)
        : endpoint(endpoint), body(body), header(header) {}
    std::string getEndpoint() const { return endpoint; }
    std::string getBody() const { return body; }
    std::string getHeader() const { return header; }
    nlohmann::json getBodyJson() const {return nlohmann::json::parse(body);}
    nlohmann::json getHeaderJson() const {return nlohmann::json::parse(header);}
    void setEndpoint(std::string endpoint) {this->endpoint = endpoint;}
    void setBody(std::string body) {this->body = body;}

private:
    std::string endpoint;
    std::string body;
    std::string header;
};
