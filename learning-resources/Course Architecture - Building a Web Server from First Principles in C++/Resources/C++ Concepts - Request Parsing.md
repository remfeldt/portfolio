# C++ Concepts - Request Parsing

This note explains what your new request files do and the C++ concepts inside them.

## Files and Responsibilities

- `request.h`: Declares the `Request` class (the shape of the object).
- `request.cpp`: Defines how the class works (actual parsing logic).

This is a common C++ pattern:
- Header (`.h`) = declarations and public API.
- Source (`.cpp`) = implementation details.

## The Request Class at a Glance

```cpp
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

Concepts here:
- `public`: members other code can access.
- `private`: internal helper functions/data.
- Constructor: `Request(const std::string& raw_request)` runs when you create an object.
- `const std::string&`:
  - `const` means the function will not modify the input string.
  - `&` means pass by reference (no copy, more efficient).

## Constructor and Delegation to parse()

```cpp
Request::Request(const std::string& raw_request) {
    parse(raw_request);
}
```

This keeps the constructor small and clear.
The constructor is responsible for initializing object state, and it delegates detailed work to `parse`.

## std::istringstream for Token Parsing

```cpp
std::istringstream stream(raw_request);
stream >> method >> path >> version;
```

`std::istringstream` lets you treat a string like an input stream.
Using `>>` extracts whitespace-separated tokens.
For a request line like:

```text
GET /index.html HTTP/1.1
```

You get:
- `method = "GET"`
- `path = "/index.html"`
- `version = "HTTP/1.1"`

## std::getline and Header Loop

```cpp
std::string line;
std::getline(stream, line);

while (std::getline(stream, line)) {
    ...
}
```

- The first `getline` consumes the remainder of the request-line newline.
- The loop then reads one header line at a time.

Pattern meaning:
- `while (std::getline(...))` continues while reads are successful.
- It stops automatically at end-of-input.

## if Statements in the Parser

Important `if` checks:

```cpp
if (!line.empty() && line.back() == '\r') {
    line.pop_back();
}
```

- HTTP lines often end in `\r\n`.
- `getline` removes `\n` but may leave `\r`.
- This check safely removes trailing `\r`.

```cpp
if (line.empty()) {
    break;
}
```

- Blank line means "headers are done" in HTTP.
- `break` exits the loop.

```cpp
size_t delimiter = line.find(": ");
if (delimiter != std::string::npos) {
    ...
}
```

- Looks for `": "` separator between header name and value.
- `std::string::npos` means "not found".

## std::unordered_map for Headers

```cpp
headers[key] = value;
```

`headers` is a hash map:
- Key = header name (e.g. `Host`)
- Value = header value (e.g. `localhost:8080`)

Why `unordered_map`:
- Average-case O(1) insertion/lookup.
- Great for "find by key" tasks like HTTP headers.

## Why Your Compile Error Happened

Your compile error:

```text
after running: g++ -std=c++20 main.cpp request.cpp -o server
main.cpp:110:9: error: unknown type name 'Request'
```

Root cause was that on-disk `request.h` and `request.cpp` were both 0-byte files, so the compiler saw no `class Request` declaration.

Now that those files contain the actual class and parser code, the type is known.

## Quick Mental Model

Think of the parser pipeline like this:

1. Build `Request` from raw HTTP text.
2. Read first line into method/path/version.
3. Read each header line.
4. Stop at blank line.
5. Store headers in a map.

This is clean, beginner-friendly C++ server code that separates network I/O (`main.cpp`) from parsing logic (`request.cpp`).
