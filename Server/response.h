#include <string>
#pragma once
class Response {
public:
    Response(std::string body, std::string header, int code)
        :body(body), header(header), code(code) {}
    Response()
        :body(""), header("") , code(0){}
    std::string getBody() const { return body; }
    std::string getHeader() const { return header; }
    int getCode() const { return code;}

private:
    std::string body;
    std::string header;
    int code;
};
