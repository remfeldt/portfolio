Excellent.

At this point, something interesting has happened.

Up until Phase 10, we've primarily been building a web server.

Starting with Phase 11, we're going to begin building the infrastructure that allows the server to grow.

This is exactly where many hobby projects either become maintainable or collapse under their own weight.

---

# 🎓 Phase 11 — Route Table Architecture

## Goal

Replace:

```cpp
if (request.path == "/status")
{
    handleStatus(client_socket);
    return;
}
```

with a route registration system.

This is the first glimpse of how frameworks like:

* Express.js
* Fastify
* ASP.NET
* Crow
* Pistache

internally think about routes.

---

# Why The Current Solution Doesn't Scale

Right now:

```cpp
void Server::routeRequest(...)
{
    if (request.path == "/status")
    {
        ...
    }

    if (request.path == "/about")
    {
        ...
    }

    if (request.path == "/projects")
    {
        ...
    }

    if (request.path == "/skills")
    {
        ...
    }

    ...
}
```

Eventually:

```txt
50 routes
100 routes
200 routes
```

becomes painful.

---

# What We Want

Instead:

```cpp
routes["/status"]
routes["/about"]
routes["/projects"]
```

Then:

```cpp
route = routes[path];
route();
```

---

# Architectural Concept

We are moving from:

```txt
Conditional Routing
```

to:

```txt
Table-Driven Routing
```

This is an important systems design pattern.

Instead of code deciding:

```txt
if this
if that
if something else
```

we use data structures.

---

# Step 1 — Add Route Storage

Open:

```cpp
server.h
```

Add:

```cpp
#include <unordered_map>
#include <functional>
```

---

Inside private:

```cpp
std::unordered_map<
    std::string,
    std::function<void(int)>
> routes;
```

---

# What Is std::function?

For now, think:

```cpp
std::function<void(int)>
```

means:

```txt
A function that receives:
    int client_socket

and returns:
    nothing
```

---

# Step 2 — Register Routes

Inside constructor:

```cpp
Server::Server(int port)
{
    this->port = port;

    routes["/status"] =
        [this](int client_socket)
        {
            handleStatus(client_socket);
        };
}
```

---

# First Lambda Encounter

This:

```cpp
[this](int client_socket)
{
    handleStatus(client_socket);
}
```

is called a lambda.

Conceptually:

```txt
anonymous function
```

Think JavaScript:

```js
(socket) => {
    handleStatus(socket);
}
```

Same idea.

---

# Step 3 — Rewrite routeRequest()

Replace:

```cpp
if (request.path == "/status")
```

with:

```cpp
auto route =
    routes.find(request.path);

if (route != routes.end())
{
    route->second(client_socket);
    return;
}
```

---

Then keep:

```cpp
serveFile(...)
```

as fallback.

---

# What Happens Now?

Request:

```txt
/status
```

becomes:

```txt
routes.find("/status")
        ↓
Found
        ↓
Execute handler
```

---

Request:

```txt
/index.html
```

becomes:

```txt
routes.find(...)
        ↓
Not Found
        ↓
serveFile(...)
```

---

# Step 4 — Add Another Route

Let's prove the architecture works.

Add:

```cpp
void handleHello(
    int client_socket
);
```

to server.h

---

Implementation:

```cpp
void Server::handleHello(
    int client_socket
)
{
    Response response;

    response.body =
        "Hello from route system!";

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

Register:

```cpp
routes["/hello"] =
    [this](int client_socket)
    {
        handleHello(client_socket);
    };
```

---

# Test

Visit:

```txt
http://localhost:8080/hello
```

Expected:

```txt
Hello from route system!
```

---

# What We Just Learned

Previously:

```txt
Request
    ↓
if statements
    ↓
handler
```

Now:

```txt
Request
    ↓
Route Table
    ↓
handler
```

This is a major architectural upgrade.

---

# Why This Matters For Your Future Portfolio

Imagine later:

```txt
/api/projects
/api/github
/api/blog
/api/resume
/api/contact
```

Adding a route becomes:

```cpp
routes["/api/projects"] = ...
```

instead of editing giant chains of conditionals.

---

# New Concept: Inversion of Control

This is subtle but important.

Previously:

```txt
routeRequest()
decided what code to run
```

Now:

```txt
Route table contains code references
```

The route table controls behavior.

This pattern appears everywhere:

* game engines
* dependency injection
* event systems
* UI frameworks
* web frameworks

---

# Suggested Cleanup (Optional)

By now you're probably noticing duplication:

```cpp
Response response;

response.body = ...
response.setHeader(...)
response.setHeader(...)

send(...)
```

appears repeatedly.

Good observation.

We're intentionally allowing that duplication to appear.

Because the next phase will solve exactly that problem.

---

# 📊 Updated Progress

After Phase 11:

```txt
[##############------]
70%
```

Roughly:

```txt
Networking             ✅
HTTP                   ✅
Request Parsing        ✅
Response Generation    ✅
File Serving           ✅
Routing                ✅
Route Registration     ✅

Response Helpers       ⏳
Repository Alignment   ⏳
Polish                 ⏳
```

---

# 🧭 Phase 12 Preview

Phase 12 introduces a very common framework concept:

```txt
Helper Methods
```

Instead of every handler building responses manually, we'll create reusable server utilities such as:

```cpp
sendResponse(...)
send404(...)
sendText(...)
```

This will dramatically reduce duplication and move us closer to the style found in mature server implementations and the original repository architecture.

*I'm ready for phase 12*