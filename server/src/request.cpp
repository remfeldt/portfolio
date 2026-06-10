#include "request.h"

#include <sstream>
#include <iostream>

Request::Request(const std::string& raw_request) {
    parse(raw_request);
}

void Request::parse(const std::string& raw_request) {

    std::istringstream stream(raw_request);

    // =====================================
    // PARSE REQUEST LINE
    // =====================================

    stream >> method >> path >> version;

    // Consume remaining newline
    std::string line;
    std::getline(stream, line);

    // =====================================
    // PARSE HEADERS
    // =====================================

    while (std::getline(stream, line)) {

        // Remove carriage return
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Empty line means end of headers
        if (line.empty()) {
            break;
        }

        size_t delimiter = line.find(": ");

        if (delimiter != std::string::npos) {

            std::string key = line.substr(0, delimiter);

            std::string value = line.substr(delimiter + 2);

            headers[key] = value;
        }
    }
}