#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>

std::string getMimeType(const std::string& path) {

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

int main() {

    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // 2. Define address
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // 3. Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // 4. Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // 5. Main accept loop
    while (true) {

        int addrlen = sizeof(address);

        int client_socket = accept(
            server_fd,
            (struct sockaddr*)&address,
            (socklen_t*)&addrlen
        );

        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "Client connected!" << std::endl;

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
            continue;
        }

        std::cout << "\n===== REQUEST =====\n";
        std::cout << buffer << std::endl;

        // =========================================
        // PARSE HTTP REQUEST LINE
        // =========================================

        std::istringstream request_stream(buffer);

        std::string method;
        std::string path;
        std::string version;

        request_stream >> method >> path >> version;

        std::cout << "Method: " << method << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Version: " << version << std::endl;

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

            std::string not_found_body = "404 Not Found";

            std::string not_found_response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(not_found_body.size()) + "\r\n"
                "\r\n" +
                not_found_body;

            send(
                client_socket,
                not_found_response.c_str(),
                not_found_response.size(),
                0
            );

            close(client_socket);
            continue;
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

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: " + mime_type + "\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        // =========================================
        // SEND RESPONSE
        // =========================================

        send(
            client_socket,
            response.c_str(),
            response.size(),
            0
        );

        // =========================================
        // CLOSE CLIENT CONNECTION
        // =========================================

        close(client_socket);
    }

    close(server_fd);

    return 0;
}