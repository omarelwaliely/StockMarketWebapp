
#include "../request.h"
#pragma once
class EndpointStrategy { //using strategy design pattern, we need to make a virtual void that acts as an interface for endpoints
public:
    virtual Response execute(const Request& request) = 0;
    virtual ~EndpointStrategy() = default;
};