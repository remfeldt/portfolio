# 🎓 Phase 10 — Routing & Request Dispatch

## Goal

Currently:

```cpp
Request request(buffer);

serveFile(
    client_socket,
    request.path
);
```

Every request becomes:

```txt
URL
 ↓
File
```

That's how a static web server thinks.

---

Modern web frameworks think differently:

```txt
Request
 ↓
Route Match
 ↓
Handler
 ↓
Response
```

For example:

```txt
GET /
        ↓
Home Handler

GET /about
        ↓
About Handler

GET /api/status
        ↓
Status Handler
```

---

# Why This Matters

Suppose you later want your portfolio to expose:

```txt
/api/projects
/api/github
/api/status
```

Those don't necessarily correspond to files.

Instead:

```txt
Request
 ↓
Code
 ↓
Generated Response
```

That's routing.

---

# Current Architecture

```txt
handleClient()
        ↓
serveFile()
```

---

# Target Architecture

```txt
handleClient()
        ↓
routeRequest()
       / \
      /   \
serveFile()  handleStatus()
```

This is the first step toward application logic.

---

# Step 1 — Update server.h

Add:

```cpp
void routeRequest(
    int client_socket,
    const Request& request
);

void handleStatus(
    int client_socket
);
```

---

# Step 2 — Create handleStatus()

In `server.cpp`:

```cpp
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
```

---

# Step 3 — Create routeRequest()

```cpp
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
```

---

# Step 4 — Update handleClient()

Replace:

```cpp
serveFile(
    client_socket,
    request.path
);
```

with:

```cpp
routeRequest(
    client_socket,
    request
);
```

---

# What We Just Built

Previously:

```txt
GET /status
      ↓
Look for ./status file
      ↓
404
```

Now:

```txt
GET /status
      ↓
Route Match
      ↓
handleStatus()
      ↓
Generated Response
```

This is the first dynamic endpoint in your server.

---

# Testing

## Browser

Visit:

```txt
http://localhost:8080/status
```

Expected:

```txt
Server is running
```

---

## curl

```bash
curl localhost:8080/status
```

Expected:

```txt
Server is running
```

---

## Existing Site

Verify:

```txt
http://localhost:8080/
```

still serves:

```txt
index.html
```

unchanged.

---

# New Architectural Concept: Dispatching

You've already learned:

```txt
Parsing
```

and

```txt
Serialization
```

Now we're introducing:

```txt
Dispatching
```

Which means:

> Determine where a request should be sent.

A dispatcher receives a request and forwards it to the appropriate handler.

---

# Why This Is Important For Your Future Portfolio

Eventually you'll want something like:

```txt
GET /api/projects
GET /api/blog
GET /api/github
GET /api/skills
```

These routes will likely generate data dynamically.

The architecture we're starting now is exactly what will make that possible.

---

# Repository Convergence

This is one of the first phases where our architecture begins looking noticeably closer to the original project's design philosophy.

We're moving away from:

```txt
file server
```

toward:

```txt
request processing pipeline
```

which is a major step toward how web frameworks and more complete HTTP servers are structured.
