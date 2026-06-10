# C++ Concepts - Response Serialization

This note explains what your `Response` files do and the C++ concepts used to turn server data into a valid HTTP response.

## Files and Responsibilities

- `response.h`: Declares the `Response` class (shape and public API).
- `response.cpp`: Defines behavior (how headers/body become raw HTTP text).

This keeps your code organized:
- Header (`.h`) = declarations and interface.
- Source (`.cpp`) = implementation details.

## The Response Class at a Glance

```cpp
class Response
{
public:
    int statusCode;
    std::string statusText;
    std::string body;
    std::unordered_map<std::string, std::string> headers;

    Response();

    void setHeader(const std::string& key, const std::string& value);
    std::string toString() const;
};
```

Concepts here:
- `public`: fields/functions accessible from other code.
- Constructor: `Response()` sets default values.
- `const std::string&`: efficient read-only string parameters.
- `toString() const`: promises it does not modify the object.

## Constructor Defaults

```cpp
Response::Response()
{
    statusCode = 200;
    statusText = "OK";
}
```

Why this is useful:
- New responses start as successful (`200 OK`) by default.
- You can override later for errors like `404 Not Found`.

## Header Storage with std::unordered_map

```cpp
std::unordered_map<std::string, std::string> headers;
```

This is a hash map:
- Key = header name (for example `Content-Type`)
- Value = header value (for example `text/html`)

Benefits:
- Fast average-case insert and lookup: $O(1)$.
- Natural fit for HTTP header key/value data.

## setHeader() Helper

```cpp
void Response::setHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}
```

What happens:
- Adds a new header if it does not exist.
- Replaces the value if that header already exists.

This gives one clear API for header updates instead of exposing insertion logic everywhere.

## std::stringstream in toString()

```cpp
std::stringstream response;
```

`std::stringstream` lets you build text incrementally with `<<`.
It is cleaner than manual string concatenation when assembling multi-line HTTP messages.

## HTTP Response Format Built by toString()

The method outputs this structure:

```text
HTTP/1.1 <statusCode> <statusText>\r\n
<header-name>: <header-value>\r\n
...more headers...\r\n
\r\n
<body>
```

In your code:
1. Write the status line.
2. Loop through headers and write each `key: value` line.
3. Write a blank line.
4. Write the body.

That blank line is required by HTTP to separate headers from body.

## Why "\r\n" Matters

HTTP uses CRLF line endings:
- `\r` = carriage return
- `\n` = newline

Even though some clients are tolerant, proper HTTP formatting should use `\r\n` for protocol lines.

## Header Loop Pattern

```cpp
for (const auto& header : headers)
{
    response << header.first << ": " << header.second << "\r\n";
}
```

Concepts:
- Range-based `for` loop over container elements.
- `const auto&` avoids copying each map entry.
- `header.first` is the key, `header.second` is the value.

## Quick Mental Model

Think of response generation as a pipeline:

1. Start with defaults (`200 OK`).
2. Set headers (`Content-Type`, maybe `Content-Length`).
3. Set body content.
4. Serialize everything to one raw HTTP string.
5. Send that string over the socket.

This mirrors the opposite direction of your `Request` parser:
- Request parser: raw text -> structured object.
- Response serializer: structured object -> raw text.

## Practical Next Improvement

A common upgrade is auto-setting `Content-Length` based on `body.size()` before serialization.
That helps clients know exactly how many bytes to read in the response body.
