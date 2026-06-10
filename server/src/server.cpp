#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "request.h"
#include "response.h"

std::string Server::getMimeType(const std::string& path) {

    if (path.ends_with(".html")) {
        return "text/html";
    }

    if (path.ends_with(".css")) {
        return "text/css";
    }

    if (path.ends_with(".js")) {
        return "application/javascript";
    }

    if (path.ends_with(".png")) {
        return "image/png";
    }

    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) {
        return "image/jpeg";
    }

    return "text/plain";
}

void Server::handleClient(
    int client_socket
)
{
    // =========================================
    // RECEIVE REQUEST
    // =========================================

    char buffer[4096] = {0};

    ssize_t bytes_received = recv(
        client_socket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytes_received < 0) {
        perror("recv failed");
        close(client_socket);
        return;
    }

    std::cout << "\n===== REQUEST =====\n";
    std::cout << buffer << std::endl;

    // =========================================
    // PARSE HTTP REQUEST LINE
    // =========================================

    Request request(buffer);

    std::cout << "Method: " << request.method << std::endl;
    std::cout << "Path: " << request.path << std::endl;
    std::cout << "Version: " << request.version << std::endl;

    for (const auto& header : request.headers) {
        std::cout << header.first << ": " << header.second << std::endl;
    }

    routeRequest(
        client_socket,
        request
    );      

}

void Server::handleStatus(
    int client_socket
)
{
    Response response;

    response.body = "Server is running";

    response.setHeader(
        "Content-Type",
        "text/plain"
    );

    response.setHeader(
        "Content-Length",
        std::to_string(response.body.size())
    );

    std::string responseText =
        response.toString();

    send(
        client_socket,
        responseText.c_str(),
        responseText.size(),
        0
    );
}

void Server::routeRequest(
    int client_socket,
    const Request& request
)
{
    if (request.path == "/status")
    {
        handleStatus(client_socket);
        return;
    }

    serveFile(
        client_socket,
        request.path
    );
}

void Server::serveFile(
    int client_socket,
    const std::string& path
)
{
    // =========================================
    // MAP URL TO FILE
    // =========================================

    std::string file_path;

    if (path == "/") {
        file_path = "index.html";
    }
    else {
        file_path = "." + path;
    }

    std::cout << "Serving file: " << file_path << std::endl;

    // =========================================
    // OPEN FILE
    // =========================================

    std::ifstream file(file_path);

    // 404 HANDLING
    if (!file.is_open()) {
        
        Response response;

        response.statusCode = 404;
        response.statusText = "Not Found";

        response.body = "404 Not Found";

        response.setHeader(
            "Content-Type",
            "text/plain"
        );

        response.setHeader(
            "Content-Length",
            std::to_string(response.body.size())
        );

        std::string responseText = response.toString();

        send(
            client_socket,
            responseText.c_str(),
            responseText.size(),
            0
        );

        return;
    }

    // =========================================
    // READ FILE CONTENTS
    // =========================================

    std::stringstream file_buffer;

    file_buffer << file.rdbuf();

    std::string body = file_buffer.str();

    // =========================================
    // BUILD HTTP RESPONSE
    // =========================================

    std::string mime_type = getMimeType(file_path);
    
    Response response;

    response.body = body;

    response.setHeader(
        "Content-Type",
        mime_type
    );

    response.setHeader(
        "Content-Length",
        std::to_string(body.size())
    );

    std::string responseText =
        response.toString();
    // =========================================
    // SEND RESPONSE
    // =========================================

    send(
        client_socket,
        responseText.c_str(),
        responseText.size(),
        0
    );
}

Server::Server(int port)
{
    this->port = port;
}

void Server::start()
{
    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket failed");
        return;
    }

    // 2. Define address
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return;
    }

    // 4. Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return;
    }

    std::cout
        << "Server listening on port "
        << port
        << std::endl;


    // 5. Main accept loop
    while (true) {

        socklen_t addrlen = sizeof(address);

        int client_socket = accept(
            server_fd,
            (struct sockaddr*)&address,
            &addrlen
        );

        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        handleClient(client_socket);

        // =========================================
        // CLOSE CLIENT CONNECTION
        // =========================================

        close(client_socket);
          
    }

    close(server_fd);

    return;
    
}