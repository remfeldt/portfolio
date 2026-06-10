Excellent.

We're now at the point where the educational server begins evolving into an actual software architecture.

Up until now, your code is probably starting to feel like:

```txt
main.cpp
 ├─ socket setup
 ├─ bind
 ├─ listen
 ├─ accept
 ├─ recv
 ├─ Request creation
 ├─ file serving
 ├─ MIME lookup
 ├─ Response creation
 └─ send
```

This is exactly what happens in real projects.

At first:

```cpp
int main()
{
    // everything
}
```

Eventually:

```cpp
Server server;
server.start();
```

becomes necessary.

---

# 🎓 Phase 8 — Server Abstraction

## Goal

Move networking responsibilities into a dedicated `Server` class.

After this phase:

```cpp
int main()
{
    Server server(8080);
    server.start();
}
```

will become the entry point.

---

# 🧠 Why This Matters

Let's look at the responsibilities currently living inside `main.cpp`.

### Networking

```cpp
socket()
bind()
listen()
accept()
recv()
send()
close()
```

---

### HTTP

```cpp
Request request(...)
Response response(...)
```

---

### Application Logic

```cpp
serve files
determine MIME type
404 handling
```

---

Three completely different concerns.

A common rule in software architecture is:

> One class should have one primary responsibility.

---

# Current Architecture

```txt
main.cpp
 ├─ TCP
 ├─ HTTP
 └─ Application
```

---

# Target Architecture

```txt
main.cpp
    ↓
Server
    ↓
Request
    ↓
Application Logic
    ↓
Response
```

---

# Project Structure

Create:

```txt
server.h
server.cpp
```

Project now becomes:

```txt
main.cpp

server.h
server.cpp

request.h
request.cpp

response.h
response.cpp

index.html
styles.css
```

---

# Step 1 — Create server.h

## server.h

```cpp
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
};
```

---

# 🧠 Design Discussion

Notice:

```cpp
port
server_fd
```

are private.

This is the first time we've enforced encapsulation.

Why?

Because nobody outside the server should care about:

```cpp
server_fd = 3
```

That's an implementation detail.

---

# Step 2 — Create server.cpp

Start simple.

## server.cpp

```cpp
#include "server.h"

#include <iostream>

Server::Server(int port)
{
    this->port = port;
}

void Server::start()
{
    std::cout
        << "Server starting on port "
        << port
        << std::endl;
}
```

Compile.

Nothing useful yet.

We're creating the skeleton first.

---

# Step 3 — Update main.cpp

Replace everything with:

```cpp
#include "server.h"

int main()
{
    Server server(8080);

    server.start();

    return 0;
}
```

Compile:

```bash
g++ -std=c++20 \
main.cpp \
server.cpp \
request.cpp \
response.cpp \
-o server
```

---

Run:

```bash
./server
```

You should see:

```txt
Server starting on port 8080
```

---

# 🧠 Why We're Doing This Incrementally

A common beginner mistake is:

```txt
Move 500 lines
Compile
Get 87 errors
Cry
```

Instead:

```txt
Create abstraction
Compile
Move one responsibility
Compile
Move next responsibility
Compile
```

Small safe steps.

---

# Step 4 — Move Socket Setup

Take this code from `main.cpp`:

```cpp
socket()
bind()
listen()
```

and move it into:

```cpp
Server::start()
```

---

Your `start()` should eventually look like:

```cpp
void Server::start()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("socket failed");
        return;
    }

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (
        bind(
            server_fd,
            (struct sockaddr*)&address,
            sizeof(address)
        ) < 0
    )
    {
        perror("bind failed");
        return;
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        return;
    }

    std::cout
        << "Server listening on port "
        << port
        << std::endl;
}
```

---

Compile.

Test.

Browser won't work yet.

That's expected.

We've only moved startup logic.

---

# Step 5 — Move Accept Loop

Now move:

```cpp
while(true)
{
    accept(...)
}
```

inside `Server::start()`.

At this stage:

```txt
main.cpp
```

contains almost nothing.

Which is exactly what we want.

---

# What main.cpp Should Eventually Become

```cpp
#include "server.h"

int main()
{
    Server server(8080);

    server.start();

    return 0;
}
```

That's it.

---

# Why This Is Important

Imagine later:

```cpp
Server portfolioServer(8080);
Server adminServer(9000);
```

or

```cpp
Server apiServer(5000);
```

You can create multiple server instances.

You cannot do that when everything lives inside `main()`.

---

# Current Limitation

Even after moving code:

```txt
Server
 ├─ networking
 ├─ file serving
 ├─ MIME handling
 ├─ request handling
 └─ response handling
```

still contains too much.

That's okay.

We're peeling layers gradually.

---

# Architectural Insight

Notice what happened over the last phases.

Phase 6:

```txt
Request abstraction
```

Phase 7:

```txt
Response abstraction
```

Phase 8:

```txt
Server abstraction
```

We're separating responsibilities one layer at a time.

This is exactly how mature codebases evolve.

Not through giant rewrites.

Through incremental extraction.

---

# Suggested Testing

For this phase:

### Browser

Verify:

```txt
http://localhost:8080
```

still works after code movement.

Behavior should be identical.

---

### curl

```bash
curl -v localhost:8080
```

Should behave exactly as before.

---

### Wireshark

Interestingly:

You should see **zero protocol changes**.

Only code organization changed.

This is a good lesson:

> Architecture changes should not alter external behavior.

---

# ✅ Phase 8 Checkpoint

You are done when:

* [X] `server.h` exists
* [X] `server.cpp` exists
* [X] `main.cpp` only creates a Server object
* [X] Browser still works
* [X] curl still works
* [X] You understand encapsulation and responsibility separation

---

# 🧭 Phase 9 Preview

Phase 9 is where we start introducing dedicated server methods such as:

```cpp
handleClient()
serveFile()
```

This is the phase where the code structure begins looking noticeably closer to the original repository rather than a teaching scaffold.

We'll start breaking the massive `start()` method into smaller units and discuss one of the most important software engineering principles in systems programming:

> Cohesion vs Coupling.

*Before we move on, can you make a small recap of what's been done so far and how far we are in the total progress of phases towards the end result of the repo*

Absolutely. This is actually a good point to pause and assess where we are architecturally before continuing.

---

# 🎓 Course Progress Recap

## Phase 1 — Raw TCP Server

### Learned

* `socket()`
* `bind()`
* `listen()`
* `accept()`

### Built

```txt
Client
   ↓
TCP
   ↓
Server
```

### Key Insight

A server is fundamentally:

```txt
Socket
   ↓
Connection
   ↓
Bytes
```

No HTTP yet.

---

## Phase 2 — Receiving Data

### Learned

* `recv()`
* kernel socket buffers
* TCP streams

### Built

```txt
Client
   ↓
HTTP Request
   ↓
recv()
```

### Key Insight

HTTP is just text sent over TCP.

---

## Phase 3 — Sending HTTP Responses

### Learned

* `send()`
* HTTP response format
* status line
* headers
* body

### Built

```txt
Request
   ↓
Response
```

### Key Insight

A browser only understands properly formatted HTTP responses.

---

## Phase 4 — Static File Serving

### Learned

* file I/O
* URL → file mapping
* 404 handling

### Built

```txt
GET /
   ↓
index.html
   ↓
Browser
```

### Key Insight

Static web servers fundamentally map URLs to files.

---

## Phase 5 — MIME Types

### Learned

* Content-Type
* browser asset loading
* CSS serving

### Built

```txt
index.html
   ↓
styles.css
   ↓
Browser
```

### Key Insight

Browsers rely on MIME metadata, not just file extensions.

---

## Phase 6 — Request Abstraction

### Learned

* parsing separation
* protocol abstraction
* CRLF handling

### Built

```txt
Raw HTTP
   ↓
Request
```

### Files Added

```txt
request.h
request.cpp
```

### Key Insight

Parsing belongs in its own layer.

---

## Phase 7 — Response Abstraction

### Learned

* serialization
* protocol generation
* response modeling

### Built

```txt
Response
   ↓
Raw HTTP
```

### Files Added

```txt
response.h
response.cpp
```

### Key Insight

Serialization is the inverse of parsing.

---

## Phase 8 — Server Abstraction

### Learned

* encapsulation
* responsibility separation
* class ownership

### Built

```txt
main.cpp
   ↓
Server
```

### Files Added

```txt
server.h
server.cpp
```

### Key Insight

Networking concerns belong in a dedicated server abstraction.

---

# Current Architecture

Today your project roughly resembles:

```txt
main.cpp

server.h
server.cpp

request.h
request.cpp

response.h
response.cpp

index.html
styles.css
```

And architecturally:

```txt
TCP Socket
      ↓
Server
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
TCP Socket
```

This is already a surprisingly respectable architecture for a learning web server.

---

# How Close Are We To The Original Repository?

Now for the important question.

When I originally inspected the repository structure, I mentally broke it into roughly:

```txt
Foundation
HTTP Abstractions
Server Architecture
Routing
File Serving
Utility Helpers
Polish
```

We've completed:

### Foundation

✔ TCP sockets

✔ Accept loop

✔ recv/send

✔ File serving

✔ MIME handling

---

### HTTP Abstractions

✔ Request class

✔ Response class

---

### Server Architecture

✔ Server class introduced

⚠ Still needs refinement

---

# Estimated Completion

If I visualize the journey:

```txt
[##########----------]
        ~50%
```

Approximately:

### 45–55% complete

depending on how closely we want to mirror the repo.

---

# What Remains

The remaining work is less about networking and more about architecture.

---

## Phase 9

Server decomposition

Extract:

```cpp
handleClient()
serveFile()
```

This will significantly clean up `Server::start()`.

---

## Phase 10

Routing layer

Something like:

```cpp
if (request.path == "/")
```

will begin evolving toward route handling.

---

## Phase 11

Utility extraction

Likely moving helper functionality out of server logic.

---

## Phase 12

Error handling improvements

Cleaner responses.

Better code organization.

---

## Phase 13

Repository convergence

This is where we'll compare our architecture against the actual repository and adjust naming, layout, and implementation details to match more closely.

---

# More Important Than Percentage

The percentage can be misleading because:

### First 50%

Contains most of the concepts:

* TCP
* sockets
* HTTP
* parsing
* serialization
* file serving

These are the hard concepts.

---

### Last 50%

Contains mostly:

* organization
* maintainability
* cleaner abstractions
* extensibility

Still important, but conceptually less difficult.

---

# If We Stopped Today

You would already understand enough to explain:

* how browsers communicate with servers
* how HTTP works
* how static file servers work
* how requests are parsed
* how responses are generated
* how sockets operate on macOS

which is honestly farther than many web developers ever go.

The remaining phases are about transforming that knowledge into a cleaner, more reusable server architecture that increasingly resembles the repository's final design.

*Lets move to the next phase*