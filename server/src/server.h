#pragma once

#include <string>
#include <unordered_map>
#include <functional>

class Request;

class Server
{
public:

    Server(int port);

    void start();

private:

    int port;
    int server_fd;

    std::unordered_map<
        std::string,
        std::function<void(int)>
    > routes;

    void handleClient(int client_socket);

    void routeRequest(
    int client_socket,
    const Request& request
    );

    void handleStatus(
        int client_socket
    );

    void handleHello(
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