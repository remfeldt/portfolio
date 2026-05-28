# 🎓 Phase 4 — Serving Real Files

> Goal:
> Transform your server from:

* “respond with hardcoded text”

into:

* “serve actual web content from disk”

This is the phase where your project becomes:

> a genuine static web server

And this is the first direct bridge toward:

* your portfolio
* frontend hosting
* browser-rendered pages

---

# 🧠 Conceptual Shift

Until now:

```txt id="jlwm3r"
request → hardcoded string
```

Now:

```txt id="n0yiy5"
request path → filesystem path → file contents → HTTP response
```

This is fundamentally what static servers do.

Examples:

* Nginx
* Apache HTTP Server
* CDN edge servers

At the core:

> they map URLs to files

---

# ⚠️ Critical Security Note (Preview)

A naive implementation is vulnerable to:

```txt id="7d51yf"
GET /../../etc/passwd
```

We will intentionally ignore this *temporarily* for learning clarity.

Later:

* path sanitization
* normalization
* root confinement

become important.

---

# 🧩 What We Add This Phase

We need:

1. Parse requested path
2. Open file from disk
3. Read contents
4. Send contents as HTTP response

---

# ⚙️ Step 1 — Create a Real HTML File

Create:

## 📄 `index.html`

```html id="t9j0ji"
<!DOCTYPE html>
<html>
<head>
    <title>My C++ Server</title>
</head>
<body>
    <h1>Hello from your C++ web server!</h1>
    <p>You are now serving real HTML files.</p>
</body>
</html>
```

Place it beside:

```txt id="5u8n8w"
main.cpp
```

---

# ⚙️ Step 2 — Extract Request Path

We need to parse:

```http id="9n48w0"
GET / HTTP/1.1
```

We only care about:

```txt id="j0e1i2"
/
```

---

# 🧠 Minimal HTTP Parsing

This is intentionally primitive.

We are NOT building:

* full parser
* RFC-compliant implementation

Yet.

We are:

> extracting enough information to serve files

---

# ⚙️ Add Headers

At top:

```cpp id="djcezv"
#include <fstream>
#include <sstream>
```

---

# ⚙️ Replace Request Handling Section

Replace everything after printing request with this:

```cpp id="nrdj4u"
// Parse request line
std::istringstream request_stream(buffer);

std::string method;
std::string path;
std::string version;

request_stream >> method >> path >> version;

std::cout << "Method: " << method << std::endl;
std::cout << "Path: " << path << std::endl;
```

---

# 🧠 Why This Works

For request:

```http id="w5p7ow"
GET / HTTP/1.1
```

Stream extraction splits on whitespace:

```txt id="9rxtr6"
method  = GET
path    = /
version = HTTP/1.1
```

---

# ⚙️ Step 3 — Map URL to File

Add:

```cpp id="g5rdo5"
std::string file_path;

if (path == "/") {
    file_path = "index.html";
} else {
    file_path = "." + path;
}
```

---

# 🧠 Why `"." + path`

If browser requests:

```txt id="gq3fn2"
/styles.css
```

We map to:

```txt id="4s71o0"
./styles.css
```

relative to current working directory.

---

# ⚙️ Step 4 — Read File

Add:

```cpp id="2vjlwm"
std::ifstream file(file_path);

if (!file.is_open()) {
    std::string not_found =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "404 Not Found";

    send(client_socket, not_found.c_str(), not_found.size(), 0);

    close(client_socket);
    continue;
}

std::stringstream file_buffer;
file_buffer << file.rdbuf();

std::string body = file_buffer.str();
```

---

# 🧠 Deep Dive: `rdbuf()`

This line:

```cpp id="pl93n5"
file_buffer << file.rdbuf();
```

Streams the entire file contents into memory.

Efficient enough for:

* small static files
* educational server

Not ideal for:

* large files
* production streaming

---

# ⚙️ Step 5 — Send HTML Response

Replace previous hardcoded response with:

```cpp id="2lww3x"
std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: " + std::to_string(body.size()) + "\r\n"
    "\r\n" +
    body;

send(
    client_socket,
    response.c_str(),
    response.size(),
    0
);
```

---

# 🌐 Test in Browser

Run:

```bash id="ww0i7t"
./server
```

Open:

```txt id="g16wr6"
http://localhost:8080
```

You should now see:

* rendered HTML
* actual webpage
* browser parsing your document

This is a major milestone.

---

# 🧪 Suggested Testing (Very Relevant Here)

## 1. Browser DevTools Network Tab

Inspect:

* response headers
* status code
* MIME type

This directly visualizes your server behavior.

---

## 2. Wireshark

Now especially interesting:

* inspect actual HTML payload
* observe TCP FIN packets after response

Filter:

```txt id="x8cwy7"
tcp.port == 8080
```

---

## 3. curl

Try:

```bash id="w1m1np"
curl -v localhost:8080
```

And:

```bash id="2ndk9n"
curl -v localhost:8080/missing.html
```

You should observe:

* `200 OK`
* `404 Not Found`

---

# ⚠️ Important Limitation

Currently:

```http id="vww7m0"
Content-Type: text/html
```

for EVERYTHING.

Meaning:

* CSS files wrong MIME
* JS files wrong MIME
* images impossible

We fix this next.

---

# 🧠 Systems-Level Insight

You have now implemented:

```txt id="gjij6m"
URL → filesystem lookup → HTTP response
```

That is:

> the central primitive of static web hosting

Your portfolio goal is now directly connected to your server architecture.

---

# 🧩 Relation to Final Repo

This phase maps heavily toward:

* static serving behavior
* route/file handling
* eventual `server.cpp` structure

Still without:

* classes
* abstractions
* MIME helpers

Those come next.

---

# ✅ Phase 4 Checkpoint

You are done when:

* [X] `index.html` renders in browser
* [X] `404` works
* [X] curl shows proper status codes
* [X] You understand:

  * URL → file mapping
  * file reading
  * response body generation

---

# 🧭 Next Phase Preview

## Phase 5 — MIME Types & Multi-Asset Serving

This is where:

* CSS works properly
* JavaScript works properly
* browser loads multiple assets correctly

We’ll implement:

* extension parsing
* content-type mapping
* serving entire frontend structure

At that point:

> your server can genuinely host a basic portfolio site

---

Proceed once:

* HTML rendering works
* 404 handling works
* and you’ve inspected responses in DevTools/Wireshark.

*Can you show me how the full server file should look now with the additions of this phase. The current server main.cpp file currently looks like this:*

```cpp #include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>

int main() {
    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // 2. Define address
    sockaddr_in address;
    address.sin_family = AF_INET;          // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Accept any IP
    address.sin_port = htons(8080);        // Port 8080

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

    // 5. Accept loop
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

        // Receive request
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

        // HTTP response body
        std::string body = "Hello from your C++ server!";

        // Construct full HTTP response
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        // Send response
        send(
            client_socket,
            response.c_str(),
            response.size(),
            0
        );

        close(client_socket);
    }

    close(server_fd);
    return 0;
}```

Yes — your current file is exactly where it should be before Phase 4 integration.
You only need to replace the “hardcoded response” section with:

* request parsing
* file loading
* dynamic response generation

Here is the **full `main.cpp` as it should look after completing Phase 4**:

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>

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

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
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
```

---

# 🧠 Important Observations Before Continuing

At this exact point your server now supports:

| Feature                | Status |
| ---------------------- | ------ |
| TCP connections        | ✅      |
| HTTP request receiving | ✅      |
| HTTP response sending  | ✅      |
| File serving           | ✅      |
| 404 handling           | ✅      |
| Browser rendering      | ✅      |

---

# ⚠️ Intentional Weaknesses (Educational)

Your server still has several important limitations.

These are GOOD right now because they expose how web servers actually evolve.

---

## 1. MIME types are wrong

Everything returns:

```http
Content-Type: text/html
```

Meaning:

* CSS broken
* JS broken
* images impossible

This is next phase.

---

## 2. Entire file loads into memory

```cpp
file_buffer << file.rdbuf();
```

Bad for:

* large video files
* huge assets

Production servers stream incrementally.

---

## 3. Only one recv()

Dangerous in real networking.

TCP does NOT guarantee:

* full request arrives in one packet

Later we discuss:

* incremental parsing
* buffering strategies

---

## 4. No persistent connections

Browser reconnects every request.

HTTP/1.1 normally prefers:

```http
Connection: keep-alive
```

---

# 🧪 Recommended Tests Before Phase 5

---

## Browser DevTools

Open:

* Network tab
* Reload page

Observe:

* request headers
* response headers
* status codes

---

## curl

Test 404:

```bash
curl -v localhost:8080/missing.html
```

---

## Wireshark

Very educational now.

Observe:

* HTML payload
* FIN/ACK teardown
* packet segmentation

Filter:

```txt
tcp.port == 8080
```

---

# 🧭 Next Phase

## Phase 5 — MIME Types & Asset Serving

This is where:

* CSS loads correctly
* JavaScript loads correctly
* your server starts resembling a real frontend host

We’ll implement:

* extension parsing
* MIME mapping
* serving multiple asset types

At that point:

> you can host a legitimate static portfolio site.

*Lets move to phase 5*

Excellent.
This is the phase where your server stops being:

“an HTML responder”

and starts becoming:

“an actual static asset server.”

This is also the phase most directly connected to your portfolio goal.