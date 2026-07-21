#pragma once

#include <string>

#include "response.h"
#include "router.h"

class Request;

class Server
{
public:

    Server(int port);

    void start();

private:

    int port;
    int server_fd;

    void handleClient(int client_socket);

    void routeRequest(
    int client_socket,
    const Request& request
    );

    void serveFile(
        int client_socket,
        const std::string& path
    );

    std::string getMimeType(
        const std::string& path
    );

    void handleStatus(
        int client_socket
    );

    void handleHello(
        int client_socket
    );

    void sendResponse(
        int client_socket,
        Response& response
    );

    void sendTextResponse(
        int client_socket,
        const std::string& body,
        const std::string& content_type
    );

    void sendErrorResponse(
        int client_socket,
        int status_code,
        const std::string& status_text,
        const std::string& body
    );

    
};