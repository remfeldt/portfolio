#pragma once

#include <string>
#include <unordered_map>

class Request {

public:

    std::string method;
    std::string path;
    std::string version;

    std::unordered_map<std::string, std::string> headers;

    Request(const std::string& raw_request);

private:

    void parse(const std::string& raw_request);
};