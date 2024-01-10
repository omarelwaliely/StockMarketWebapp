#include <string>
#pragma once
class Request {
public:
    Request(std::string endpoint, std::string body, std::string header)
        : endpoint(endpoint), body(body), header(header) {}
    std::string getEndpoint() const { return endpoint; }
    std::string getBody() const { return body; }
    std::string getHeader() const { return header; }

private:
    std::string endpoint;
    std::string body;
    std::string header;
};
