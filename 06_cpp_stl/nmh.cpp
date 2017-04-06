#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>
#include "json.hpp"


/**
 @link: https://developer.chrome.com/extensions/nativeMessaging
 */

using json = nlohmann::json;

namespace ns {
    struct request {
        std::string type;
        std::string property;
        std::vector<std::string> args;
    };

    void to_json(json &j, const ns::request &request) {
        j = json{{"type",     request.type},
                 {"property", request.property},
                 {"args",     request.args}};
    }

    void from_json(const json &j, ns::request &request) {
        try {
            request.type = j.at("type").get<std::string>();
        } catch (nlohmann::detail::out_of_range &ex) {}
        try {
            request.property = j.at("property").get<std::string>();
        } catch (nlohmann::detail::out_of_range &ex) {}
        try {
            request.args = j.at("args").get<std::vector<std::string>>();
        } catch (nlohmann::detail::out_of_range &ex) {}
    }
}

std::ostream &operator<<(std::ostream &output, const std::vector<std::string> &list) {
    output << "[";
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it > list.begin())
            output << ", ";
        output << (*it);
    }
    output << "]";
    return output;
}

std::ostream &operator<<(std::ostream &output, const ns::request &request) {
    output << "[type: " << request.type << ", property: " << request.property << ", args: " << request.args << "]";
    return output;
}

void test() {
    ns::request request = {"command", "cmd", {"/k", "echo"}};
    json json_request = request;
    std::cout << json_request << std::endl;
    ns::request deserialized_request{json_request["type"], json_request["property"], json_request["args"]};
    std::cout << deserialized_request << std::endl;
}


int
main(int argc, char **argv) {
    while (std::cin.good()) {
        std::string string_request;
        ns::request request;

        std::getline(std::cin, string_request);
        json json_request = json::parse(string_request);
        ns::from_json(json_request, request);
        std::cout << json_request << std::endl;
    }
    test();
    return 0;
}

