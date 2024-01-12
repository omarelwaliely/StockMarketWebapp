#include "endpointStrategy.h"
#pragma once
class ProcessedChange: public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};