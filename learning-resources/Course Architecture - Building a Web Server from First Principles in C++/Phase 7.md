Excellent.

Up to now we've built:

```txt
TCP Socket
    ↓
recv()
    ↓
Request Parser
    ↓
Request Object
    ↓
Application Logic
```

But the outgoing side still looks like this:

```cpp
std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: " + std::to_string(body.size()) + "\r\n"
    "\r\n" +
    body;
```

We're manually constructing protocol text every time.

That's exactly the same situation we had before introducing `Request`.

---

# 🎓 Phase 7 — Response Abstraction

> Goal:
>
> Create a `Response` class that is responsible for:
>
> * HTTP status codes
> * Headers
> * Body content
> * Serialization into HTTP format

This phase maps conceptually to the `response.*` files in the final repository.

---

# 🧠 Why We Need This

Imagine six months from now.

You add:

```cpp
200 OK
404 Not Found
500 Internal Server Error
301 Redirect
```

Without abstraction:

```cpp
std::string response =
    "HTTP/1.1 404 Not Found\r\n"
    ...
```

gets duplicated everywhere.

The moment duplication appears, maintenance costs start rising.

---

# Current Architecture

Currently:

```txt
Request Object
     ↓
main.cpp
     ↓
Manual HTTP String Building
     ↓
send()
```

---

# Target Architecture

We want:

```txt
Request Object
     ↓
Application Logic
     ↓
Response Object
     ↓
Response::toString()
     ↓
send()
```

Notice how this mirrors the request side.

---

# New Project Structure

Create:

```txt
response.h
response.cpp
```

Your project should now look like:

```txt
main.cpp
request.h
request.cpp
response.h
response.cpp
index.html
styles.css
```

---

# Step 1 — Create response.h

```cpp
#pragma once

#include <string>
#include <unordered_map>

class Response
{
public:

    int statusCode;
    std::string statusText;
    std::string body;

    std::unordered_map<std::string, std::string> headers;

    Response();

    void setHeader(
        const std::string& key,
        const std::string& value
    );

    std::string toString() const;
};
```

---

# Design Discussion

Notice:

```cpp
headers
```

exists on both:

```cpp
Request
Response
```

This symmetry is intentional.

Most HTTP frameworks expose:

```cpp
request.headers
response.headers
```

because HTTP itself is symmetric.

---

# Step 2 — Create response.cpp

```cpp
#include "response.h"

#include <sstream>

Response::Response()
{
    statusCode = 200;
    statusText = "OK";
}

void Response::setHeader(
    const std::string& key,
    const std::string& value
)
{
    headers[key] = value;
}

std::string Response::toString() const
{
    std::stringstream response;

    response
        << "HTTP/1.1 "
        << statusCode
        << " "
        << statusText
        << "\r\n";

    for (const auto& header : headers)
    {
        response
            << header.first
            << ": "
            << header.second
            << "\r\n";
    }

    response << "\r\n";

    response << body;

    return response.str();
}
```

---

# 🧠 New Concept: Serialization

Earlier we discussed parsing:

```txt
Raw HTTP
    ↓
Request Object
```

Now we're doing the reverse:

```txt
Response Object
    ↓
Raw HTTP
```

This process is called:

> Serialization

A serializer converts structured data into a transmissible format.

---

# Step 3 — Include Response in main.cpp

Add:

```cpp
#include "response.h"
```

---

# Step 4 — Replace 404 Response

Currently:

```cpp
std::string not_found_response =
    ...
```

Replace with:

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

std::string responseText = response.toString();

send(
    client_socket,
    responseText.c_str(),
    responseText.size(),
    0
);
```

---

# Step 5 — Replace 200 Response

Current:

```cpp
std::string response =
```

becomes:

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

std::string responseText =
    response.toString();

send(
    client_socket,
    responseText.c_str(),
    responseText.size(),
    0
);
```

---

# Compile

Now compile:

```bash
g++ -std=c++20 \
main.cpp \
request.cpp \
response.cpp \
-o server
```

---

# Testing

## Browser

Verify:

```txt
/
/styles.css
```

still load correctly.

Nothing should change from the browser's perspective.

---

## curl

Run:

```bash
curl -v localhost:8080
```

You should still see:

```http
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: ...
```

---

## Wireshark

This is actually a great phase to inspect with Wireshark.

You should observe:

```txt
Response Object
      ↓
Serialized HTTP Text
      ↓
TCP Payload
```

The packet contents should be identical to Phase 5.

That's important.

Good abstractions change code structure without changing protocol behavior.

---

# What You Just Learned

You now understand the opposite side of Phase 6:

### Parsing

```txt
Raw HTTP
    ↓
Request Object
```

### Serialization

```txt
Response Object
    ↓
Raw HTTP
```

These are two of the most common patterns in network software.

---

# Architectural Result

Your server is beginning to resemble this:

```txt
Socket Layer
    ↓
Request Parser
    ↓
Request Object
    ↓
Application Logic
    ↓
Response Object
    ↓
Response Serializer
    ↓
Socket Layer
```

This is a major step toward maintainable server architecture.

---

# ✅ Phase 7 Checkpoint

You are done when:

* [X] `response.h` exists
* [X] `response.cpp` exists
* [X] Browser behavior is unchanged
* [X] `curl -v` still shows valid responses
* [X] 404 responses use `Response`
* [X] 200 responses use `Response`
* [X] You understand serialization as the inverse of parsing

---

# 🧭 Next Phase Preview

Phase 8 is where we start introducing a **Server abstraction** and move responsibility out of `main.cpp`.

This is the point where the architecture begins visibly converging toward the structure of the original repository rather than remaining a teaching prototype.

*Lets go on to the next phase*