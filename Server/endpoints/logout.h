#include "endpointStrategy.h"
#pragma once
class Logout : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};