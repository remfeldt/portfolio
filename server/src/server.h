#pragma once

#include <string>

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

    void handleStatus(
        int client_socket
    );

    void serveFile(
        int client_socket,
        const std::string& path
    );

    std::string getMimeType(
        const std::string& path
    );
};