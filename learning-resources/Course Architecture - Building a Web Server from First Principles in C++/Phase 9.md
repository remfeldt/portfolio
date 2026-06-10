Excellent.

We're now entering the phase where many software engineering principles become visible in the code itself.

Up until now, we've mostly been learning networking and HTTP. Beginning with Phase 9, we're learning something equally important:

> How to prevent working code from becoming unmaintainable code.

---

# 🎓 Phase 9 — Server Decomposition

## Goal

Break the large `Server::start()` function into smaller methods with clear responsibilities.

Currently, your server probably resembles:

```txt
Server::start()
 ├─ socket creation
 ├─ bind
 ├─ listen
 ├─ accept
 ├─ recv
 ├─ Request parsing
 ├─ file lookup
 ├─ MIME detection
 ├─ Response creation
 └─ send
```

It works.

But it's beginning to violate an important principle:

> A function should do one thing well.

---

# 🧠 The Problem With Large Functions

Imagine returning to this code six months from now.

You want to add:

```txt
POST support
Logging
Authentication
Caching
```

Where do you put it?

If everything is inside:

```cpp
Server::start()
```

the function becomes:

```txt
500+ lines
```

and eventually becomes difficult to reason about.

---

# A Better Architecture

We want:

```txt
Server::start()
    ↓
handleClient()
    ↓
serveFile()
```

Each function has a specific purpose.

---

# New Target Structure

We'll gradually move toward:

```txt
Server
 ├─ start()
 ├─ handleClient()
 ├─ serveFile()
 └─ getMimeType()
```

Notice how the names describe responsibilities.

---

# Step 1 — Update server.h

Add new private methods:

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

    void handleClient(int client_socket);

    void serveFile(
        int client_socket,
        const std::string& path
    );

    std::string getMimeType(
        const std::string& path
    );
};
```

---

# 🧠 Design Discussion

Notice that:

```cpp
getMimeType()
```

is moving into `Server`.

Why?

Because MIME detection is part of serving HTTP resources.

At this stage we don't need a dedicated utility class yet.

We'll keep responsibilities grouped until they naturally need extraction.

---

# Step 2 — Extract getMimeType()

Move your current helper function:

```cpp
std::string getMimeType(...)
```

inside `Server`.

Implementation:

```cpp
std::string Server::getMimeType(
    const std::string& path
)
{
    if (path.ends_with(".html"))
        return "text/html";

    if (path.ends_with(".css"))
        return "text/css";

    if (path.ends_with(".js"))
        return "application/javascript";

    if (path.ends_with(".png"))
        return "image/png";

    if (path.ends_with(".jpg"))
        return "image/jpeg";

    if (path.ends_with(".jpeg"))
        return "image/jpeg";

    return "text/plain";
}
```

---

# Step 3 — Extract serveFile()

Create:

```cpp
void Server::serveFile(
    int client_socket,
    const std::string& path
)
{
}
```

Initially empty.

Compile.

Ensure everything still builds.

---

# Why Create Empty Functions First?

This is a professional refactoring technique.

Bad approach:

```txt
Move 100 lines
Compile
Get 40 errors
```

Good approach:

```txt
Create function
Compile

Move logic
Compile

Move next logic
Compile
```

Small safe steps.

---

# Step 4 — Move File Serving Logic

Move all of this:

```cpp
file_path determination
ifstream open
404 response
file reading
response creation
send()
```

into:

```cpp
Server::serveFile()
```

The result should conceptually look like:

```cpp
void Server::serveFile(
    int client_socket,
    const std::string& path
)
{
    std::string file_path;

    if (path == "/")
    {
        file_path = "index.html";
    }
    else
    {
        file_path = "." + path;
    }

    // open file
    // generate 404 if needed
    // read contents
    // create Response
    // send response
}
```

---

# 🧠 Why This Is Better

Before:

```txt
start()
 ├─ networking
 ├─ HTTP
 ├─ file serving
```

After:

```txt
start()
 ├─ networking

serveFile()
 ├─ file serving
```

Responsibilities become separated.

---

# Step 5 — Extract handleClient()

Add:

```cpp
void Server::handleClient(
    int client_socket
)
{
}
```

---

Move:

```cpp
recv()
Request creation
debug logging
serveFile(...)
```

into:

```cpp
handleClient()
```

---

Conceptually:

```cpp
void Server::handleClient(
    int client_socket
)
{
    char buffer[4096] = {0};

    recv(...);

    Request request(buffer);

    serveFile(
        client_socket,
        request.path
    );
}
```

---

# Step 6 — Simplify start()

Now the accept loop becomes:

```cpp
while (true)
{
    int client_socket = accept(...);

    if (client_socket < 0)
    {
        continue;
    }

    handleClient(client_socket);

    close(client_socket);
}
```

---

# Compare Before & After

## Before

```txt
start()
 ├─ accept
 ├─ recv
 ├─ parse
 ├─ file lookup
 ├─ MIME
 ├─ response
 └─ send
```

---

## After

```txt
start()
 ├─ accept
 └─ handleClient()

handleClient()
 ├─ recv
 ├─ parse
 └─ serveFile()

serveFile()
 ├─ file lookup
 ├─ MIME
 ├─ response
 └─ send
```

This is a huge architectural improvement.

---

# 🧠 New Concept: Cohesion

One of the most important software engineering concepts.

High cohesion means:

```txt
A function contains code that belongs together.
```

Examples:

### Good

```txt
serveFile()
 ├─ file lookup
 ├─ file read
 └─ response generation
```

Everything relates to serving files.

---

### Bad

```txt
serveFile()
 ├─ file lookup
 ├─ user authentication
 ├─ socket creation
 └─ logging
```

Unrelated responsibilities.

Low cohesion.

---

# 🧠 New Concept: Coupling

Coupling measures:

> How dependent two pieces of code are on each other.

We want:

```txt
Low coupling
High cohesion
```

That combination produces maintainable systems.

---

# Testing

This phase should produce **zero observable behavior changes**.

Use:

### Browser

```txt
http://localhost:8080
```

---

### curl

```bash
curl -v localhost:8080
```

---

### Wireshark

Packets should be identical.

Again:

> Refactoring changes architecture, not behavior.

---

# Current Architecture

After Phase 9:

```txt
main.cpp
   ↓
Server

Server
 ├─ start()
 ├─ handleClient()
 ├─ serveFile()
 └─ getMimeType()

Request

Response
```

This is now starting to resemble a small framework rather than a networking experiment.

---

# ✅ Phase 9 Checkpoint

You are done when:

* [ ] `getMimeType()` is a Server method
* [ ] `serveFile()` exists
* [ ] `handleClient()` exists
* [ ] `start()` only handles server lifecycle
* [ ] Browser still works
* [ ] curl still works
* [ ] You understand cohesion and coupling

---

# 🧭 Phase 10 Preview

Phase 10 introduces something that appears in almost every web framework:

```cpp
route request
    ↓
handler
    ↓
response
```

We'll stop thinking purely in terms of files and begin thinking in terms of **request handling**, which is a major conceptual step toward how frameworks like Express, Fastify, and many C++ web servers are structured.

*Everything works but I want you to give feedback on the implementation as I fixed a few things that wasn't mentioned in your previous response, like removing continue as it was no longer within a while loop. Please tell if anything needs to change:*

*Server.cpp: [server.cpp](/server/src/server.cpp)*

This is actually quite good. More importantly, the fixes you made show that you're starting to reason about the architecture rather than blindly following instructions, which is exactly the goal of this course.

I'll review it the same way I would review a university networking assignment or a junior developer's pull request.

---

# Overall Assessment

Current architecture:

```txt
main
 ↓
Server::start()

Server::start()
 ↓
handleClient()

handleClient()
 ↓
Request

handleClient()
 ↓
serveFile()

serveFile()
 ↓
Response
```

This is a solid improvement over Phase 8.

I'd rate it:

```txt
Architecture:      8/10
Readability:       8/10
Correctness:       8/10
Maintainability:   7/10
Repository Match:  6/10
```

The remaining gap is mostly code organization rather than functionality.

---

# Good Fix #1

You removed the old `continue`.

Excellent.

When we extracted logic into methods, the old:

```cpp
continue;
```

inside:

```cpp
while(true)
```

became:

```cpp
return;
```

inside:

```cpp
handleClient()
```

which is the correct semantic equivalent.

That is exactly the kind of thing that happens during refactoring.

---

# Good Fix #2

You moved most responsibilities appropriately

Current breakdown:

```txt
start()
    accept clients

handleClient()
    receive request
    parse request

serveFile()
    file serving
    response creation
```

This is good cohesion.

---

# Issue #1 — Double Close

This is the biggest thing I'd change right now.

Inside:

```cpp
serveFile()
```

404 path:

```cpp
close(client_socket);
return;
```

But later:

```cpp
handleClient()
returns
```

Then:

```cpp
start()
```

executes:

```cpp
close(client_socket);
```

again.

---

# What Happens?

On macOS/Linux:

```cpp
close()
```

on an already closed descriptor is undefined from your application's perspective.

Usually:

```cpp
EBADF
```

(Bad File Descriptor)

or silent failure.

---

# Fix

Remove:

```cpp
close(client_socket);
```

from:

```cpp
serveFile()
```

---

Only:

```cpp
start()
```

should own client lifetime.

---

# Ownership Principle

A very important systems programming rule:

> Whoever opens a resource should usually close it.

Currently:

```cpp
accept()
```

occurs inside:

```cpp
start()
```

Therefore:

```cpp
close()
```

should occur there too.

---

# Issue #2 — Response Class Not Used For 200

You correctly use:

```cpp
Response
```

for:

```cpp
404
```

But for:

```cpp
200
```

you're back to:

```cpp
std::string response =
```

manual HTTP construction.

---

Current:

```cpp
std::string response =
    "HTTP/1.1 200 OK\r\n"
```

---

Should become:

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
```

Then send:

```cpp
responseText
```

instead.

---

Why?

Consistency.

Every response should be created through the same abstraction.

---

# Issue #3 — Shadowed server_fd

In:

```cpp
void Server::start()
{
    int server_fd = socket(...)
}
```

you create:

```cpp
local server_fd
```

But your class already contains:

```cpp
int server_fd;
```

---

Meaning:

```cpp
this->server_fd
```

is never used.

---

Fix:

Replace:

```cpp
int server_fd = socket(...)
```

with:

```cpp
server_fd = socket(...)
```

---

Otherwise the class member serves no purpose.

---

# Issue #4 — addrlen Type

Current:

```cpp
int addrlen = sizeof(address);
```

Then:

```cpp
(socklen_t*)&addrlen
```

---

More idiomatic:

```cpp
socklen_t addrlen = sizeof(address);
```

Then:

```cpp
&addrlen
```

No cast required.

---

Small improvement.

Not urgent.

---

# Issue #5 — Unused Variable

You create:

```cpp
std::string not_found_body =
    "404 Not Found";
```

but never use it.

---

Remove:

```cpp
not_found_body
```

---

Small cleanup.

---

# Issue #6 — File Reading Mode

Current:

```cpp
std::ifstream file(file_path);
```

opens text mode.

---

For future image support:

```cpp
std::ifstream file(
    file_path,
    std::ios::binary
);
```

is safer.

---

Not required yet.

But worth knowing.

---

# Issue #7 — recv() == 0

Current:

```cpp
if (bytes_received < 0)
```

handles errors.

---

But:

```cpp
recv()
```

returns:

```cpp
0
```

when peer disconnects.

---

Future improvement:

```cpp
if (bytes_received <= 0)
```

---

Not critical right now.

---

# Architectural Observation

Something interesting has happened.

Originally:

```txt
main.cpp
    250+ lines
```

Now:

```txt
start()
handleClient()
serveFile()
```

Each function has a clear purpose.

That means cohesion has improved significantly.

---

# Relation To Final Repository

At this point we have implemented roughly:

```txt
Networking
HTTP Request
HTTP Response
Basic File Serving
```

We're no longer learning networking fundamentals.

We're now refining architecture.

The next major step is:

```txt
Request
    ↓
Routing
    ↓
Handler
    ↓
Response
```

which is where web frameworks begin to emerge.

---

# Changes I Would Make Before Phase 10

### Definitely do

✅ Remove `close(client_socket)` from `serveFile()`

✅ Use `Response` for 200 responses

✅ Use class member `server_fd`

---

### Nice to have

✅ `socklen_t addrlen`

✅ remove unused variable

✅ `recv() <= 0`

---

### Can wait

⏳ binary file reading

⏳ image support

⏳ keep-alive

⏳ routing

⏳ thread pool

If you make those three "Definitely do" fixes, I'd consider this phase successfully completed and cleaner than the version I originally outlined.

*Update the current progress bar of the project after this and lets move to the next phase*

Before Phase 10, here's the updated progress estimate.

# 📊 Progress Update

When we started:

```txt
[--------------------]
0%
```

After completing Phase 9 and the architectural cleanup:

```txt
[############--------]
60%
```

Approximately:

```txt
Foundation & Networking       ✅ 100%
HTTP Protocol Basics          ✅ 100%
Request Parsing               ✅ 100%
Response Generation           ✅ 100%
Static File Serving           ✅ 100%
MIME Handling                 ✅ 100%
Server Abstraction            ✅ 100%
Server Decomposition          ✅ 100%

Routing Layer                 ⏳ 0%
Request Dispatching           ⏳ 0%
Repository Structure Match    ⏳ 25%
Polish & Refinement           ⏳ 0%
```

---

# What Has Actually Been Accomplished

What's important is that we've completed most of the *hard concepts*.

You now understand:

```txt
TCP sockets
HTTP messages
Parsing
Serialization
Static serving
MIME types
Class abstractions
Encapsulation
Cohesion
Coupling
```

Those are the concepts that typically take students the longest to internalize.

---

# What Remains

The remaining work is more architectural than conceptual.

We're moving from:

```txt
Web Server
```

toward:

```txt
Mini Web Framework
```

The original repository isn't just serving files.

It's beginning to introduce a request-processing pipeline.