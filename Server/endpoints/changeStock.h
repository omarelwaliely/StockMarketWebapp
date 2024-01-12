#include "endpointStrategy.h"
#pragma once
class ChangeStock : public EndpointStrategy {
public:
    Response execute(const Request& request) override;
};