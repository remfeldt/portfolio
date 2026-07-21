#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "request.h"
#include "response.h"

Router router;

Server::Server(int port)
{
    this->port = port;
    
    

    router.addRoute(
        "/status",
        [this](int client_socket)
        {
            handleStatus(client_socket);
        }
    );
    router.addRoute(
        "/hello",
        [this](int client_socket)
        {
            handleHello(client_socket);
        }
    );
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

void Server::routeRequest(
    int client_socket,
    const Request& request
)
{
    Router::Handler* route =
    router.findRoute(request.path);

    if (route != nullptr)
    {
        (*route)(client_socket);
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
    if (!file.is_open())
    {
        sendErrorResponse(
            client_socket,
            404,
            "Not Found",
            "404 Not Found"
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

    sendResponse(
        client_socket,
        response
    );
}


void Server::handleStatus(
    int client_socket
)
{
    sendTextResponse(
        client_socket,
        "Server is running",
        "text/plain"
    );
}

void Server::handleHello(
    int client_socket
)
{
    sendTextResponse(
        client_socket,
        "Hello from route system!",
        "text/plain"
    );
}

void Server::sendTextResponse(
    int client_socket,
    const std::string& body,
    const std::string& content_type
)
{
    Response response;

    response.body = body;

    response.setHeader(
        "Content-Type",
        content_type
    );

    response.setHeader(
        "Content-Length",
        std::to_string(body.size())
    );

    sendResponse(
        client_socket,
        response
    );
}

void Server::sendErrorResponse(
    int client_socket,
    int status_code,
    const std::string& status_text,
    const std::string& body
)
{
    Response response;

    response.statusCode = status_code;
    response.statusText = status_text;
    response.body = body;

    response.setHeader(
        "Content-Type",
        "text/plain"
    );

    response.setHeader(
        "Content-Length",
        std::to_string(body.size())
    );

    sendResponse(
        client_socket,
        response
    );
}

void Server::sendResponse(
    int client_socket,
    Response& response
)
{
    std::string responseText =
        response.toString();

    send(
        client_socket,
        responseText.c_str(),
        responseText.size(),
        0
    );
}

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