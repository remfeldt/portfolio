#pragma once

#include <string>

class Server
{
public:

    Server(int port);

    void start();

private:

    int port;
    int server_fd;

    void handleClient(int client_socket);

    void serveFile(
        int client_socket,
        const std::string& path
    );

    std::string getMimeType(
        const std::string& path
    );
};