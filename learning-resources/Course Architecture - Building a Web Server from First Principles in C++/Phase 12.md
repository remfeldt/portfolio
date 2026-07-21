# 🎓 Phase 12 — Response Helper Methods

We're now going to remove the duplication that has appeared in your route handlers.

At the moment, a handler probably looks like this:

```cpp
void Server::handleStatus(int client_socket)
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
```

And `handleHello()` likely contains almost exactly the same structure.

Only this changes:

```cpp
response.body = "...";
```

That is a sign that the repeated behavior should be extracted.

---

# 🧠 The Problem: Duplication

Currently:

```txt
handleStatus()
 ├─ create Response
 ├─ set body
 ├─ set Content-Type
 ├─ set Content-Length
 ├─ serialize
 └─ send

handleHello()
 ├─ create Response
 ├─ set body
 ├─ set Content-Type
 ├─ set Content-Length
 ├─ serialize
 └─ send
```

The actual difference is only:

```txt
response body
```

So we want:

```txt
handleStatus()
      ↓
sendTextResponse()

handleHello()
      ↓
sendTextResponse()
```

---

# 🏗️ Target Architecture

We'll add:

```cpp
sendResponse()
```

and:

```cpp
sendTextResponse()
```

Conceptually:

```txt
Handler
   ↓
Helper
   ↓
Response
   ↓
send()
```

---

# Step 1 — Add Helper Methods to `server.h`

Add these to the private section:

```cpp
void sendResponse(
    int client_socket,
    Response& response
);

void sendTextResponse(
    int client_socket,
    const std::string& body,
    const std::string& content_type
);
```

Because `Response` is now used in the header, include:

```cpp
#include "response.h"
```

Your relevant `server.h` should look approximately like:

```cpp
#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "response.h"

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

    void handleClient(
        int client_socket
    );

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
};
```

⚠️ One important detail: because `routeRequest()` uses `Request`, `server.h` also needs access to the `Request` declaration. You can either include:

```cpp
#include "request.h"
```

or use a forward declaration:

```cpp
class Request;
```

For this stage, I recommend simply:

```cpp
#include "request.h"
```

This makes the header easier to understand.

---

# Step 2 — Implement `sendResponse()`

In `server.cpp`:

```cpp
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
```

This method now owns the process of:

```txt
Response Object
      ↓
Serialization
      ↓
TCP send()
```

---

# Step 3 — Implement `sendTextResponse()`

Now create a higher-level helper:

```cpp
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
```

Notice the hierarchy:

```txt
sendTextResponse()
        ↓
sendResponse()
        ↓
response.toString()
        ↓
send()
```

This is an abstraction ladder.

---

# Step 4 — Refactor `handleStatus()`

Replace the entire current implementation with:

```cpp
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
```

That is much clearer.

The function now expresses its intent:

> Send a text response saying that the server is running.

It no longer needs to know:

* how `Response` works
* how `Content-Length` is calculated
* how serialization works
* how `send()` works

---

# Step 5 — Refactor `handleHello()`

Similarly:

```cpp
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
```

---

# Step 6 — Refactor 404 Handling

Your current 404 logic can also use the helper.

Instead of manually constructing:

```cpp
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
```

we could create a status-aware helper:

```cpp
void sendErrorResponse(
    int client_socket,
    int status_code,
    const std::string& status_text,
    const std::string& body
);
```

Add it to `server.h`:

```cpp
void sendErrorResponse(
    int client_socket,
    int status_code,
    const std::string& status_text,
    const std::string& body
);
```

Then implement:

```cpp
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
```

---

# Step 7 — Simplify `serveFile()`

Your 404 block now becomes:

```cpp
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
```

Much cleaner.

And importantly:

```txt
serveFile()
```

is now responsible for:

> Deciding that a file was not found.

But it is no longer responsible for:

> Manually constructing the entire HTTP response.

That responsibility belongs to the response helper.

---

# Step 8 — Use `Response` For Successful Files

Your successful response should also use the same abstraction:

```cpp
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
```

Now every response follows:

```txt
Response
    ↓
sendResponse()
    ↓
serialize
    ↓
send()
```

---

# Final Responsibility Structure

After this phase:

```txt
Server
│
├── start()
│   └── Server lifecycle
│
├── handleClient()
│   └── Client communication
│
├── routeRequest()
│   └── Route dispatch
│
├── serveFile()
│   └── File lookup and reading
│
├── handleStatus()
│   └── Status endpoint logic
│
├── handleHello()
│   └── Hello endpoint logic
│
├── sendTextResponse()
│   └── Convenient text response creation
│
├── sendErrorResponse()
│   └── Error response creation
│
└── sendResponse()
    └── Response serialization and transmission
```

This is becoming a real layered architecture.

---

# 🧠 The Important Concept: Abstraction Levels

Consider:

```cpp
sendTextResponse(
    client_socket,
    "Server is running",
    "text/plain"
);
```

This is a **high-level operation**.

It hides:

```txt
Create Response
    ↓
Set body
    ↓
Set Content-Type
    ↓
Set Content-Length
    ↓
Serialize
    ↓
send()
```

This is called **abstraction**.

The higher-level code expresses *what* it wants:

> Send a text response.

The lower-level helper knows *how* to do it.

---

# Why This Is Especially Relevant To Your Future JavaScript Goal

You eventually want to expose selected C++ functionality to JavaScript.

A future API might look conceptually like:

```javascript
server.get("/status", (request, response) => {
    response.send("Server is running");
});
```

That JavaScript API should not expose:

```txt
socket()
bind()
send()
Content-Length
HTTP serialization
```

Instead, it should expose a high-level interface.

The helper architecture we're creating now is the foundation for that.

---

# 🧪 Testing This Phase

Run your usual tests:

### Static files

```bash
curl -v localhost:8080/
```

### Status route

```bash
curl -v localhost:8080/status
```

### Hello route

```bash
curl -v localhost:8080/hello
```

### Missing file

```bash
curl -v localhost:8080/does-not-exist
```

You should verify that the actual HTTP responses remain unchanged.

Wireshark is also useful here: the packets should remain functionally identical despite the internal refactoring.

---

# 📊 Updated Progress

After Phase 12:

```txt
[###############-----]
75%
```

The remaining work is increasingly about making the server more reusable and converging toward the final repository architecture.

The broad progression is now:

```txt
Raw TCP Server
      ↓
HTTP Server
      ↓
Static File Server
      ↓
Structured HTTP Server
      ↓
Route-Based Server
      ↓
Reusable Server Architecture
```

---

# ✅ Phase 12 Checkpoint

You should now have:

* [X] `sendResponse()`
* [X] `sendTextResponse()`
* [X] `sendErrorResponse()`
* [X] All handlers using shared response helpers
* [X] `serveFile()` using `Response`
* [X] No duplicated response serialization logic
* [X] No duplicated `send()` logic
* [X] Browser and curl behavior unchanged

The next phase will be a significant architectural step: **moving route registration out of the `Server` constructor and toward a more reusable routing interface**. This is where the server starts becoming something that an application developer could configure rather than something whose routes are hard-coded into the server implementation.
