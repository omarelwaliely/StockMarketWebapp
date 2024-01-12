#include <string>
#include "json.hpp"
#pragma once
class Response {
public:
    Response(std::string body, std::string header, int code)
        :body(body), header(header), code(code) {}
    Response()
        :body(""), header("") , code(0){}
    std::string getBody() const { return body; }
    std::string getHeader() const { return header; }
    nlohmann::json getBodyJson() const {return nlohmann::json::parse(body);}
    nlohmann::json getHeaderJson() const {return nlohmann::json::parse(header);}
    int getCode() const { return code;}

private:
    std::string body;
    std::string header;
    int code;
};
