#include "endpointStrategy.h"
#pragma once
class Register : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};
