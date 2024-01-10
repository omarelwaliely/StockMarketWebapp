#include "endpointStrategy.h"
#pragma once
class NotFound : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};