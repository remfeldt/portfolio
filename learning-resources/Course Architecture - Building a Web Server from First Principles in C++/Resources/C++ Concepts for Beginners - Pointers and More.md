Read [](file:///Users/alex-work/Projects/portfolio/server/src/main.cpp)

Great file to learn from — it's a real TCP server, so every line has a purpose. Let me walk through it concept by concept.

---

## `#include` — Headers

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
```

Just like `using` in C#, `#include` pulls in external code. These are **header files** — they declare functions and types so the compiler knows they exist.

- `<iostream>` — gives you `std::cout` (console output)
- `<sys/socket.h>`, `<netinet/in.h>` — POSIX networking types/functions (BSD sockets API)
- `<unistd.h>` — Unix functions like `close()`
- `<cstring>` — C string utilities like `memset()`

There's no project file or `.csproj` — headers are just text that gets copy-pasted into your file during compilation.

---

## `int main()` — Entry Point

```cpp
int main() {
    ...
    return 0;
}
```

Same as C# `Main()`. Returning `0` means success; anything else signals an error to the OS. You'll see `return 1` used here when something goes wrong.

---

## Variables and Basic Types

```cpp
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
```

C++ is statically typed like C#. `int` is a 32-bit integer. Here `server_fd` holds a **file descriptor** — on Unix/macOS, sockets are represented as integers (a small number the OS uses to track open resources).

---

## `if` Statements and Error Checking

```cpp
if (server_fd == -1) {
    perror("socket failed");
    return 1;
}
```

`if` works identically to C#. The Unix convention is that functions return `-1` on failure (not exceptions), so C/C++ code is littered with these checks. `perror()` reads the global `errno` variable and prints a human-readable error message to `stderr`.

---

## Structs

```cpp
sockaddr_in address;
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(8080);
```

`sockaddr_in` is a **struct** — like a C# `struct` or a plain data class with no methods. You access fields with `.` (dot notation), same as C#. `AF_INET`, `INADDR_ANY` etc. are pre-defined constants (like C# `const` or `enum` values).

`htons()` converts the port number to **network byte order** (big-endian). Different CPUs store multi-byte numbers differently; this ensures the bytes are in the order the network expects.

---

## Pointers and Casting — the Trickiest Part

```cpp
bind(server_fd, (struct sockaddr*)&address, sizeof(address))
```

This line has three C++ pointer concepts packed in:

**1. The `&` operator — "address of"**

`&address` means *"give me a pointer to `address`"*. A pointer is a variable that holds a memory address rather than a value. In C# you mostly never think about this; in C++ it's everywhere.

```
address       → the actual struct in memory
&address      → the memory address where that struct lives (a pointer)
```

**2. The cast `(struct sockaddr*)`**

`bind()` is a generic function that accepts any socket address type. It expects a `sockaddr*` pointer, but we have a `sockaddr_in*`. This cast says *"trust me, treat this pointer as if it points to a `sockaddr`"*. It's similar to casting in C# but much less safe — the compiler won't stop you from casting incorrectly.

**3. `sizeof(address)`**

Returns the size of the struct in bytes at compile time — like `Marshal.SizeOf()` in C#. `bind()` needs this because `sockaddr` can vary in size depending on the address family.

---

## The same pattern again — `accept`

```cpp
int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
```

`(socklen_t*)&addrlen` — here `&addrlen` takes the address of the integer `addrlen`, then casts it to a `socklen_t*`. `accept()` will **write into** `addrlen` through that pointer to tell you how big the client's address was. This is the C way of returning multiple values from a function — pass a pointer to where you want the output written. In C# you'd use `out int addrlen` for the same idea.

---

## `std::cout` and `std::endl`

```cpp
std::cout << "Server listening on port 8080..." << std::endl;
```

`std::cout` is the standard output stream — equivalent to `Console.Write()` in C#. The `<<` operator feeds data into the stream (you can chain them). `std::endl` flushes the buffer and writes a newline.

The `std::` prefix is the **namespace**. `cout` lives in the `std` namespace, just like `Console` lives in `System` in C#. You can write `using namespace std;` at the top to drop the prefix, but it's considered bad practice in larger projects.

---

## `while (true)` — Infinite Loop

```cpp
while (true) {
    ...
    if (client_socket < 0) {
        continue;  // skip to next iteration
    }
    close(client_socket);
}
```

A server needs to run forever — `while (true)` is the standard pattern. `continue` skips the rest of the current loop body and starts the next iteration (same as C#).

---

## `close()` — Manual Resource Management

```cpp
close(client_socket);
...
close(server_fd);
```

This is the biggest C++ difference from C#. There is **no garbage collector**. OS resources like sockets must be manually released. `close()` tells the OS you're done with the file descriptor. If you forget, you leak resources until the process exits.

In modern C++ you'd often wrap resources in RAII classes (like `std::unique_ptr`) that close automatically when they go out of scope — C#'s `using`/`IDisposable` was inspired by this pattern.

---

## Quick Reference Summary

| Concept | C++ | C# Equivalent |
|---|---|---|
| `#include` | Pull in header files | `using` namespace |
| `int x = fn()` | Return value from function | Same |
| `&x` | Pointer to x (address-of) | `ref`/`out` |
| `(Type*)ptr` | Unsafe pointer cast | Unsafe cast |
| `struct Foo` | Plain data type | `struct`/record |
| `std::cout <<` | Print to stdout | `Console.Write()` |
| `std::` prefix | Namespace | `System.` |
| `close()` | Release OS resource | `.Dispose()` |
| `return -1` convention | Signal error | `throw Exception` |

The biggest mindset shift coming from C#: **you are closer to the metal**. No runtime, no GC, no exceptions by default — just you, the OS, and memory addresses.