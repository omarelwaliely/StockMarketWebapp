#include "notFound.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"

Response NotFound::execute(const Request& request) {
    nlohmann::json body = {
                {"status", "failed"},
                {"message", "page not found"}
            };
    Response res("encryptedsecretkey", body.dump(), 404);
    return res;
}
