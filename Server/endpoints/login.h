#include "endpointStrategy.h"
#pragma once
class Login : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};