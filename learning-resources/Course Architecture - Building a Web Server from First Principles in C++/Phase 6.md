Excellent.
You’ve now reached the point where most people stop.

What you currently have is:

* a functioning TCP server
* basic HTTP parsing
* static asset serving
* MIME handling

Now we begin the architectural transition.

---

# 🎓 Phase 6 — Request Parsing Abstraction

> Goal:
> Separate:

* HTTP parsing logic
  from:
* server connection logic

This is where your codebase starts becoming:

> maintainable systems software

And this phase maps directly toward:

* `request.h`
* `request.cpp`

from your final repo.

---

# 🧠 Why Abstractions Become Necessary

Your current `main.cpp` is starting to accumulate responsibilities:

| Responsibility        | Current Location |
| --------------------- | ---------------- |
| TCP socket management | `main.cpp`       |
| HTTP parsing          | `main.cpp`       |
| file serving          | `main.cpp`       |
| MIME logic            | `main.cpp`       |
| response generation   | `main.cpp`       |

This eventually becomes:

* unreadable
* difficult to extend
* difficult to debug

Real systems evolve by:

> isolating responsibilities

---

# 🧩 What We’re Building

We want this:

```cpp id="w4q1zx"
Request request(buffer);

std::cout << request.method << std::endl;
std::cout << request.path << std::endl;
```

Instead of:

```cpp id="uk5nwn"
std::istringstream request_stream(buffer);
```

---

# 🧠 Systems-Level Insight

This abstraction is important because:

> parsing protocols is a separate concern from networking

Your socket layer should not care about:

* GET
* headers
* URLs

It should only:

* move bytes

---

# 📁 New Project Structure

Create:

```txt id="db5u76"
request.h
request.cpp
```

Your project now starts resembling real C++ architecture.

---

# ⚙️ Step 1 — Create `request.h`

## 📄 `request.h`

```cpp id="t6jlwm"
#pragma once

#include <string>
#include <unordered_map>

class Request {

public:

    std::string method;
    std::string path;
    std::string version;

    std::unordered_map<std::string, std::string> headers;

    Request(const std::string& raw_request);

private:

    void parse(const std::string& raw_request);
};
```

---

# 🧠 Important Design Discussion

---

## Why public fields?

For now:

* simplicity
* educational clarity

Later:

* getters
* immutability
* validation

could matter.

---

## Why `unordered_map`?

Headers are:

```http id="hjlwmr"
Key: Value
```

Perfect for:

```cpp id="gjjz0t"
unordered_map<string, string>
```

Fast lookup:

```cpp id="3ljlwm"
request.headers["Host"]
```

---

# ⚙️ Step 2 — Create `request.cpp`

## 📄 `request.cpp`

```cpp id="hgjjlwm"
#include "request.h"

#include <sstream>
#include <iostream>

Request::Request(const std::string& raw_request) {
    parse(raw_request);
}

void Request::parse(const std::string& raw_request) {

    std::istringstream stream(raw_request);

    // =====================================
    // PARSE REQUEST LINE
    // =====================================

    stream >> method >> path >> version;

    // Consume remaining newline
    std::string line;
    std::getline(stream, line);

    // =====================================
    // PARSE HEADERS
    // =====================================

    while (std::getline(stream, line)) {

        // Remove carriage return
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Empty line means end of headers
        if (line.empty()) {
            break;
        }

        size_t delimiter = line.find(": ");

        if (delimiter != std::string::npos) {

            std::string key = line.substr(0, delimiter);

            std::string value = line.substr(delimiter + 2);

            headers[key] = value;
        }
    }
}
```

---

# 🧠 Deep Parsing Discussion

---

# Why Remove `\r`

HTTP lines terminate with:

```txt id="tjlwmr"
\r\n
```

But:

```cpp id="njlwmr"
getline()
```

only removes:

```txt id="djlwmr"
\n
```

Leaving:

```txt id="9jlwmr"
\r
```

at end of line.

This is a subtle but VERY important protocol detail.

---

# Header Parsing Logic

For:

```http id="2jlwmr"
Host: localhost:8080
```

We split on:

```txt id="jlwmr"
": "
```

Result:

```txt id="wjlwmr"
key   = Host
value = localhost:8080
```

---

# ⚙️ Step 3 — Include Request Class in `main.cpp`

At top:

```cpp id="kjlwmr"
#include "request.h"
```

---

# ⚙️ Step 4 — Replace Manual Parsing

Remove:

```cpp id="8jlwmr"
std::istringstream request_stream(buffer);

std::string method;
std::string path;
std::string version;

request_stream >> method >> path >> version;
```

Replace with:

```cpp id="ljlwmr"
Request request(buffer);
```

---

# ⚙️ Step 5 — Update Usage

Replace:

```cpp id="1jlwmr"
path
```

with:

```cpp id="bjlwmr"
request.path
```

---

# ⚙️ Example

Replace:

```cpp id="4jlwmr"
if (path == "/")
```

with:

```cpp id="qjlwmr"
if (request.path == "/")
```

---

# ⚙️ Add Debug Output

Add:

```cpp id="pjlwmr"
std::cout << "Method: " << request.method << std::endl;
std::cout << "Path: " << request.path << std::endl;
std::cout << "Version: " << request.version << std::endl;

for (const auto& header : request.headers) {
    std::cout << header.first << ": " << header.second << std::endl;
}
```

---

# ⚙️ Compile

Now compile BOTH files:

```bash id="hjlwmq"
g++ -std=c++20 main.cpp request.cpp -o server
```

---

# 🧠 Systems-Level Insight

You’ve now created:

> your first protocol abstraction layer

This is extremely important architecturally.

Your codebase is evolving into:

```txt id="3jlwmq"
Network Layer
    ↓
HTTP Parsing Layer
    ↓
Application Logic
```

That separation is foundational in systems engineering.

---

# 🧪 Recommended Testing

---

## curl

Especially useful now:

```bash id="jjlwmq"
curl -v localhost:8080
```

Observe:

* headers parsed correctly
* Host/User-Agent extracted

---

## Browser DevTools

Inspect:

* request headers
* compare against your parsed output

---

## Wireshark

Now valuable for correlating:

* raw packet payload
* parsed Request object

This is excellent protocol debugging practice.

---

# ⚠️ Important Current Weaknesses

---

## 1. Header parsing is fragile

Fails on:

* malformed headers
* multiline headers
* duplicate headers

---

## 2. No body parsing

POST requests not handled yet.

---

## 3. Entire request assumed in one recv()

Still dangerous in real networking.

---

# 🧩 Relation to Final Repo

This phase maps directly to:

* `request.h`
* `request.cpp`

You are now beginning convergence toward repo architecture.

---

# ✅ Phase 6 Checkpoint

You are done when:

* [X] `Request` object works
* [X] Headers parse correctly
* [X] `main.cpp` no longer manually parses request line
* [X] Multi-file compilation works
* [X] You understand:

  * protocol abstraction
  * parsing separation
  * CRLF handling

---

# 🧭 Next Phase Preview

## Phase 7 — Response Abstraction

We now do for responses what we did for requests.

Instead of:

```cpp id="xjlwmq"
manual string concatenation
```

we’ll build:

```cpp id="yjlwmq"
Response response;
response.setHeader(...);
response.setBody(...);
```

This maps directly toward:

* `response.h`
* `response.cpp`

and moves your server toward:

> framework-like architecture.

*Before moving onto Phase 7, please expand on the following:*
*You understand:*

*protocol abstraction*
*parsing separation*
*CRLF handling*

*Using the current iteration in your descriptions/examples.*

Absolutely. These three concepts are some of the most important ideas we've encountered so far, because they're not specific to HTTP—they appear throughout systems programming, networking, game networking, distributed systems, database drivers, and operating systems.

I'll explain them using your current implementation.

---

# 1. Protocol Abstraction

## What is a protocol?

A protocol is simply:

> An agreed-upon format for communication.

Your server currently speaks HTTP.

When the browser sends:

```http
GET /styles.css HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0

```

those bytes are not random.

The browser and server both agree that:

```txt
METHOD PATH VERSION
Header: Value
Header: Value

Body
```

has meaning.

That agreement is the protocol.

---

## Before Phase 6

Your code looked roughly like:

```cpp
recv(...);

std::istringstream request_stream(buffer);

std::string method;
std::string path;
std::string version;

request_stream >> method >> path >> version;
```

The problem is:

```txt
Socket Code
  +
HTTP Parsing
  +
Application Logic
```

all exist in the same location.

Your networking layer knows HTTP details.

---

## After Phase 6

Now:

```cpp
Request request(buffer);
```

The server doesn't care how HTTP parsing works anymore.

The server only knows:

```cpp
request.path
request.method
request.headers
```

The parsing details have been hidden.

---

## Real-world Analogy

Imagine a mailroom.

Without abstraction:

```txt
Mailroom Worker
  reads envelope
  opens letter
  interprets letter
  decides what to do
```

Everything is mixed together.

With abstraction:

```txt
Mailroom Worker
  receives envelope
      ↓
Parser
      ↓
Structured Document
      ↓
Application
```

Responsibilities become separated.

---

## Why This Matters Later

Imagine adding support for:

```http
POST /login
```

or

```http
PUT /api/user
```

Without abstraction:

You edit parsing logic throughout the server.

With abstraction:

You only improve:

```cpp
Request::parse()
```

and the rest of the server continues working.

---

## In Your Current Architecture

Currently:

```txt
recv()
   ↓
buffer
   ↓
Request object
   ↓
main.cpp logic
```

That's your first protocol abstraction layer.

---

# 2. Parsing Separation

This is closely related, but slightly different.

Protocol abstraction asks:

> "How do we represent protocol data?"

Parsing separation asks:

> "Where should protocol interpretation occur?"

---

## What Is Parsing?

Parsing means:

> Transforming raw data into structured data.

You receive:

```http
GET /index.html HTTP/1.1
Host: localhost
```

as:

```cpp
char buffer[4096];
```

The computer sees:

```txt
71 69 84 32 47 105 110 ...
```

(raw bytes)

The parser turns that into:

```cpp
request.method = "GET";
request.path = "/index.html";
request.version = "HTTP/1.1";
```

---

## Why Separate Parsing?

Imagine your current server grows.

You add:

```cpp
POST
PUT
DELETE
Cookies
Query Strings
Authentication
```

Now imagine parsing code scattered throughout:

```cpp
main.cpp
server.cpp
router.cpp
```

It becomes impossible to reason about.

---

## Good Architecture

Instead:

```txt
Raw Bytes
    ↓
Request Parser
    ↓
Request Object
    ↓
Application Logic
```

Application code never touches raw HTTP.

---

## Real Example

Current:

```cpp
Request request(buffer);

if (request.path == "/")
{
    file_path = "index.html";
}
```

Notice:

The file-serving logic doesn't know:

* what `recv()` is
* what CRLF is
* how headers are parsed

That's parsing separation working correctly.

---

## Why This Is Important For Your Future Goal

When you later expose C++ functionality to JavaScript:

```js
server.onRequest((req) => {
    console.log(req.path);
});
```

you'll still want:

```txt
Network Layer
    ↓
Parser
    ↓
Request Object
    ↓
JavaScript
```

The same architecture scales upward.

---

# 3. CRLF Handling

This is the most protocol-specific topic we've encountered so far.

---

## What Is CRLF?

CRLF means:

```txt
Carriage Return + Line Feed
```

or:

```txt
\r\n
```

---

## Historical Background

Typewriters had two operations:

### Carriage Return

```txt
\r
```

Move cursor to beginning of line.

---

### Line Feed

```txt
\n
```

Move cursor down one line.

---

HTTP inherited this convention.

Therefore HTTP specifies:

```txt
\r\n
```

as line endings.

---

## What Browser Sends

A browser request actually looks like:

```txt
GET / HTTP/1.1\r\n
Host: localhost:8080\r\n
User-Agent: Mozilla\r\n
Accept: */*\r\n
\r\n
```

Notice the final:

```txt
\r\n
```

blank line.

That signals:

> End of headers.

---

## Why Your Parser Removes `\r`

Consider:

```cpp
std::getline(stream, line);
```

getline removes:

```txt
\n
```

but leaves:

```txt
\r
```

So:

```txt
Host: localhost\r
```

remains in memory.

---

## What Happens If You Don't Remove It?

Suppose:

```cpp
headers["Host"]
```

contains:

```txt
localhost\r
```

Now:

```cpp
if(headers["Host"] == "localhost")
```

fails.

Because:

```txt
"localhost\r"
!=
"localhost"
```

Even though visually they look identical.

---

## Your Current Fix

You added:

```cpp
if (!line.empty() && line.back() == '\r') {
    line.pop_back();
}
```

This removes the trailing CR character.

Now:

```txt
Host: localhost\r
```

becomes:

```txt
Host: localhost
```

which is what you actually want.

---

## Why Wireshark Is Valuable Here

If you inspect a request packet in Wireshark, you'll literally see:

```txt
GET / HTTP/1.1\r\n
Host: localhost:8080\r\n
Connection: keep-alive\r\n
\r\n
```

The protocol becomes much easier to understand because you're seeing exactly what your parser is processing.

---

# Putting It All Together

Your current request flow is now:

```txt
TCP Socket
    ↓
recv()
    ↓
Raw HTTP Bytes
    ↓
Request::parse()
        ├─ Parse Request Line
        ├─ Parse Headers
        └─ Remove CRLF Artifacts
    ↓
Request Object
        ├─ method
        ├─ path
        ├─ version
        └─ headers
    ↓
Application Logic
        └─ Serve File
```

That's a surprisingly professional architecture already.

In fact, if you look at the internals of many web frameworks, you'll find the same high-level flow:

```txt
Socket
    ↓
Parser
    ↓
Request Object
    ↓
Application
    ↓
Response Object
    ↓
Serializer
    ↓
Socket
```

And the next phase—Response Abstraction—completes the other half of that model. Once we have both `Request` and `Response`, the architecture begins to look very similar to the structure in the final repository.

*Good, lets begin the next phase*