# 🎓 Phase 1 — Raw TCP Server (Deep Systems-Level)

> Goal:  
> Understand and implement the **lowest-level working server** using POSIX sockets — no abstractions, no HTTP, just raw TCP.

By the end:

- You’ll accept real connections from your OS
- You’ll understand exactly what happens between your machine and a client

---

# 🧠 Mental Model First (Critical)

Before touching code, you need a _precise model_ of what’s happening.

## What is a socket really?

A socket is:

> A file descriptor representing a communication endpoint

On macOS (Unix-based), **everything is a file descriptor**:

- files
- pipes
- sockets

So when you create a socket, you’re getting:

int sockfd → an OS-managed handle

---

## Server lifecycle (exact sequence)

This is the canonical flow — memorize it:

socket()  
  ↓  
bind()  
  ↓  
listen()  
  ↓  
accept()  
  ↓  
recv() / send()  
  ↓  
close()

---

## What each step _really_ does

### 1. `socket()`

- Asks kernel: _“give me a communication endpoint”_
- Returns file descriptor

---

### 2. `bind()`

- Attaches socket to:
    - IP address
    - Port

👉 Without this, OS doesn’t know where to route traffic

---

### 3. `listen()`

- Marks socket as **passive**
- OS creates a **connection queue**

---

### 4. `accept()`

- Blocks until a client connects
- Returns a **new socket** (important!)

👉 You now have:

- server socket (listening)
- client socket (communication)

---

### 5. `recv()` / `send()`

- Raw byte exchange

---

# ⚙️ Implementation (Minimal, but Correct)

We’re writing this as a **single file first**  
(no classes yet — that comes later)

## 📄 `main.cpp`
```c++
#include <iostream>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <unistd.h>  
#include <cstring>  
  
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
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);  
  
        if (client_socket < 0) {  
            perror("accept failed");  
            continue;  
        }  
  
        std::cout << "Client connected!" << std::endl;  
  
        // Close immediately (we'll handle data in next phase)  
        close(client_socket);  
    }  
  
    close(server_fd);  
    return 0;  
}
```

---

# 🧪 Compile & Run (macOS M1)

g++ main.cpp -o server  
./server

---

# 🔍 Test It (Important)

In another terminal:

nc localhost 8080

Or:

telnet localhost 8080

👉 You should see:

Client connected!

---

# 🧠 Deep Dive (What Just Happened)

### When you ran `nc localhost 8080`:

1. OS creates a client socket
2. TCP handshake happens:
    - SYN
    - SYN-ACK
    - ACK
3. Your server’s `accept()` unblocks
4. Kernel gives you a **new file descriptor**

---

## 🔑 Critical Insight

This line:

int client_socket = accept(...)

Is the **moment your server becomes real**

You now have:

- One socket listening
- One socket per client

👉 This is the foundation of:

- concurrency
- scaling
- request handling

---

# ⚠️ Subtle but Important Details

### 1. `htons()`

address.sin_port = htons(8080);

- Converts host byte order → network byte order (big endian)
- Required for cross-platform correctness

---

### 2. `INADDR_ANY`

address.sin_addr.s_addr = INADDR_ANY;

Means:

> “accept connections from any network interface”

---

### 3. Blocking behavior

- `accept()` blocks
- Your server is currently:
    - single-threaded
    - one connection at a time

---

# 🧩 Where This Maps to Your Final Repo

This phase corresponds to the **core logic inside**:

- `server.cpp`

But:

- currently **no abstraction**
- later becomes `Server` class

---

# ✅ Phase 1 Checkpoint

You are done when:

- [X]  Server compiles
- [X]  Runs without crashing
- [X]  Accepts connections
- [X]  Logs “Client connected!”

---

# 🧭 Next Phase Preview

## Phase 2 — Reading Data

We’ll:

- use `recv()`
- inspect raw HTTP requests
- see what a browser actually sends

👉 This is where things start getting _very interesting_

---

# Before moving on

Tell me:

- Did it compile and run?
- Did `nc` connect successfully?

If anything behaved unexpectedly, we debug it at the syscall level.

*I succesfully compiled, ran the server and recieved client connected when ysing nc localhost 8000, lets move to phase 2*

Excellent. That means:

socket creation worked
bind worked
listen queue worked
TCP handshake completed
accept() returned successfully

You now have a real network server.

## Relevant Learning Material
[[C++ Concepts for Beginners - Pointers and More]]
