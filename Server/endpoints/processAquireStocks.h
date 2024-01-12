#include "endpointStrategy.h"
#pragma once
class ProcessAquireStocks : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};