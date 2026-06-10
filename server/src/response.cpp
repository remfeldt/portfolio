#include "response.h"

#include <sstream>

Response::Response()
{
    statusCode = 200;
    statusText = "OK";
}

void Response::setHeader(
    const std::string& key,
    const std::string& value
)
{
    headers[key] = value;
}

std::string Response::toString() const
{
    std::stringstream response;

    response
        << "HTTP/1.1 "
        << statusCode
        << " "
        << statusText
        << "\r\n";

    for (const auto& header : headers)
    {
        response
            << header.first
            << ": "
            << header.second
            << "\r\n";
    }

    response << "\r\n";

    response << body;

    return response.str();
}