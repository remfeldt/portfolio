# 🎓 Phase 2 — Reading Data from the Client

> Goal:
> Understand exactly what a client/browser sends over TCP.

This is the phase where:

* TCP becomes HTTP
* raw bytes become meaningful protocol data

---

# 🧠 Critical Concept First

Your server currently:

* accepts a connection
* immediately closes it

But browsers don’t just “connect.”

They send:

```http id="9njlwm"
GET / HTTP/1.1
Host: localhost:8080
User-Agent: ...
Accept: ...
```

HTTP is just:

> Plain text sent over TCP

That’s one of the most important realizations in networking.

---

# ⚠️ Important Distinction

Your socket:

* does NOT understand HTTP
* does NOT know what a GET request is

It only:

* receives bytes

YOU implement the protocol interpretation.

That’s what web servers fundamentally do.

---

# 🧩 What We Add This Phase

We’ll introduce:

```cpp id="bc3v0e"
recv()
```

This syscall:

* copies bytes from kernel socket buffer
* into your application memory

---

# 🧠 Kernel-Level Mental Model

When client sends data:

```txt id="g65ymu"
Client
  ↓
Network stack
  ↓
Kernel socket buffer
  ↓
recv()
  ↓
Your program buffer
```

The kernel temporarily stores incoming bytes until you read them.

---

# ⚙️ Updated Implementation

Replace your accept loop with this:

```cpp id="uhysmk"
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

    // Buffer for incoming data
    char buffer[4096] = {0};

    // Receive data
    ssize_t bytes_received = recv(
        client_socket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytes_received < 0) {
        perror("recv failed");
    } else {
        std::cout << "\n===== REQUEST START =====\n";
        std::cout << buffer << std::endl;
        std::cout << "===== REQUEST END =====\n";
    }

    close(client_socket);
}
```

---

# 🧠 Deep Dive Into `recv()`

## Signature

```cpp id="brq7gs"
ssize_t recv(
    int sockfd,
    void *buf,
    size_t len,
    int flags
);
```

---

## Parameter breakdown

---

### `sockfd`

```cpp id="kn4f85"
client_socket
```

The connected client socket returned from `accept()`.

NOT the listening socket.

---

### `buf`

```cpp id="9v5rd9"
buffer
```

Destination memory where bytes are copied.

---

### `len`

```cpp id="ynyyx9"
sizeof(buffer) - 1
```

Maximum bytes to read.

We subtract 1 because:

* we want null terminator
* so we can safely print as C-string

---

### `flags`

```cpp id="3l7nt4"
0
```

Default behavior.

Advanced servers later use:

* `MSG_DONTWAIT`
* `MSG_PEEK`

etc.

---

# ⚠️ Very Important: `recv()` Is NOT Message-Based

This is critical.

TCP is:

> a byte stream

NOT:

> discrete messages

Meaning:

* one `recv()` may get:

  * full request
  * partial request
  * multiple requests

Real HTTP parsers must handle this carefully.

Your current server:

* assumes request fits in one recv()
* acceptable for educational/simple server

---

# 🧪 Test With `nc`

Run server.

Then:

```bash id="h1z1uj"
nc localhost 8080
```

Type manually:

```http id="l2yq26"
GET / HTTP/1.1
Host: localhost
```

Press ENTER twice.

You should see request printed in server terminal.

---

# 🌐 Test With Browser (More Interesting)

Now open:

```txt id="8tw1e5"
http://localhost:8080
```

You’ll likely see browser loading forever.

That’s GOOD.

Why?

Because:

* browser sent request
* your server never responded

But now inspect terminal.

You’ll see a REAL browser request.

Likely something like:

```http id="09g1ha"
GET / HTTP/1.1
Host: localhost:8080
Connection: keep-alive
User-Agent: Mozilla/5.0 ...
Accept: text/html...
```

---

# 🧠 Important Observations

Notice:

---

## 1. HTTP is text

You can literally read it.

This is why:

* HTTP debugging is approachable
* tools like curl are powerful

---

## 2. Headers exist

Each line after request line:

```http id="omdclj"
Header: Value
```

These become extremely important later.

---

## 3. Blank line terminates headers

This:

```http id="rd1p8r"
```

(endline twice)

Marks:

> end of HTTP headers

---

# ⚠️ Why Browser Spins Forever

Your browser expects:

* valid HTTP response

But server:

* accepts request
* prints request
* closes socket

No HTTP response was sent.

Next phase fixes that.

---

# 🔬 Deep Systems-Level Insight

At this point:

* you have already built the core of:

  * nginx
  * apache
  * express internals

Everything else is:

* abstraction
* optimization
* protocol compliance

But conceptually:

> this is the foundation

---

# 🧩 Relation to Final Repo

This phase corresponds to:

* raw request handling before abstraction

Eventually becomes:

* `Request` parsing
* `Server::handleClient()`

---

# ✅ Phase 2 Checkpoint

You are done when:

* [X] `recv()` successfully reads request
* [X] Browser request prints correctly
* [X] You understand:

  * TCP stream behavior
  * HTTP is plain text
  * why browser hangs

---

# 🧭 Next Phase Preview

## Phase 3 — Sending a Valid HTTP Response

We’ll:

* use `send()`
* construct real HTTP response
* make browser successfully render content

This is the moment your server becomes:

> a functioning web server

---

Proceed once:

* you tested browser request output
* and saw raw HTTP headers successfully.

*Okay everything works, also tested with curl. Now I would like to move to phase 3*

Excellent.
Once you see raw browser requests in your terminal, you’ve crossed an important boundary:

Your program is no longer “just a TCP server”
It is now participating in the HTTP protocol.

Now we make the browser happy.