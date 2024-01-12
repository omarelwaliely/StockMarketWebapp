#include "endpointStrategy.h"
#pragma once
class AquireStocks : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};