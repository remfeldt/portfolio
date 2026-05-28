# 🎓 Phase 5 — MIME Types & Multi-Asset Serving

> Goal:
> Serve:

* HTML
* CSS
* JavaScript
* images (basic)

with correct `Content-Type` headers so browsers interpret assets correctly.

---

# 🧠 Critical Concept First

Browsers do NOT determine file meaning purely from extension.

The server must explicitly tell browser:

```http id="8t67y1"
Content-Type: text/css
```

Otherwise browser may:

* refuse execution
* refuse styling
* download file instead
* treat content incorrectly

---

# 🧩 What Are MIME Types?

MIME:

> Multipurpose Internet Mail Extensions

Originally email-related, later adopted by HTTP.

Examples:

| File    | MIME                     |
| ------- | ------------------------ |
| `.html` | `text/html`              |
| `.css`  | `text/css`               |
| `.js`   | `application/javascript` |
| `.png`  | `image/png`              |

---

# ⚠️ Current Problem

Your server currently sends:

```http id="tvok4m"
Content-Type: text/html
```

for EVERYTHING.

Meaning:

```txt id="qh1hnz"
styles.css
```

is interpreted as HTML.

Browser refuses to apply stylesheet correctly.

---

# 🧠 Real Browser Behavior

When browser loads:

```html id="f7k6gm"
<link rel="stylesheet" href="/styles.css">
```

It:

1. makes SECOND HTTP request
2. expects:

   ```http
   Content-Type: text/css
   ```

Your server must now support:

> multiple sequential asset requests

---

# ⚙️ Step 1 — Create Supporting Files

---

## 📄 `styles.css`

```css id="pdn00m"
body {
    background-color: #111;
    color: white;
    font-family: Arial, sans-serif;
    padding: 40px;
}

h1 {
    color: #00ffcc;
}
```

---

## 📄 Update `index.html`

```html id="yjlwmr"
<!DOCTYPE html>
<html>

<head>
    <title>My C++ Server</title>
    <link rel="stylesheet" href="/styles.css">
</head>

<body>

    <h1>Hello from your C++ web server!</h1>

    <p>
        Your server is now serving CSS correctly.
    </p>

</body>

</html>
```

---

# 🧠 Observe What Browser Will Do

Browser now requests:

```txt id="h9yk7v"
GET /
```

THEN:

```txt id="el42kz"
GET /styles.css
```

This is your first real multi-asset webpage.

---

# ⚙️ Step 2 — MIME Type Detection

We need:

```txt id="zq8lhw"
file extension → MIME type
```

---

# ⚙️ Add Helper Function

Place ABOVE `main()`:

```cpp id="kqnr2z"
std::string getMimeType(const std::string& path) {

    if (path.ends_with(".html")) {
        return "text/html";
    }

    if (path.ends_with(".css")) {
        return "text/css";
    }

    if (path.ends_with(".js")) {
        return "application/javascript";
    }

    if (path.ends_with(".png")) {
        return "image/png";
    }

    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) {
        return "image/jpeg";
    }

    return "text/plain";
}
```

---

# ⚠️ Important macOS / Compiler Note

`ends_with()` requires:

* C++20

Compile with:

```bash id="1k1hyh"
g++ -std=c++20 main.cpp -o server
```

---

# 🧠 Systems-Level Observation

Real servers maintain:

* huge MIME tables
* sometimes hundreds of types

You’re implementing the same idea manually.

---

# ⚙️ Step 3 — Use MIME Function

Find:

```cpp id="s4bs25"
std::string response =
```

Before it, add:

```cpp id="ljsq77"
std::string mime_type = getMimeType(file_path);
```

---

# ⚙️ Replace Response Construction

Replace:

```cpp id="8xw1vl"
"Content-Type: text/html\r\n"
```

with:

```cpp id="avjlwm"
"Content-Type: " + mime_type + "\r\n"
```

---

# ✅ Final Response Section

Should now look like:

```cpp id="vlt9nq"
std::string mime_type = getMimeType(file_path);

std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: " + mime_type + "\r\n"
    "Content-Length: " + std::to_string(body.size()) + "\r\n"
    "\r\n" +
    body;
```

---

# 🌐 Test in Browser

Reload:

```txt id="8yjlwm"
http://localhost:8080
```

You should now see:

* styled page
* dark background
* colored header

Your server is now handling:

* multiple requests
* different asset types
* MIME correctness

This is a huge milestone.

---

# 🧪 Recommended Testing (Especially Valuable Here)

---

## 1. Browser DevTools → Network Tab

Observe:

* browser requesting CSS separately
* MIME types returned
* response sizes

This is extremely educational.

---

## 2. curl MIME Validation

```bash id="mjlwmr"
curl -I localhost:8080/styles.css
```

You should see:

```http id="b7pzzk"
Content-Type: text/css
```

---

## 3. Wireshark

Now VERY interesting.

Observe:

* sequential requests
* browser parallelism behavior
* separate TCP sessions

---

# ⚠️ Important Architectural Weaknesses

Now becoming more relevant.

---

## 1. Entire file still loaded into memory

Bad for:

* images
* video
* large assets

Production servers:

* stream chunks
* use zero-copy optimizations

---

## 2. Binary files not truly handled correctly yet

Current implementation:

```cpp id="vlv1x9"
std::stringstream
```

works best for text.

Images may partially work accidentally.

Later:

* binary-safe reading matters

---

## 3. No caching headers

Real servers send:

```http id="hyjlwm"
Cache-Control
ETag
Last-Modified
```

---

# 🧠 Deep Systems Insight

At this point your server now performs:

```txt id="ndm3gi"
HTTP request
  ↓
URL parsing
  ↓
Filesystem lookup
  ↓
MIME classification
  ↓
HTTP response generation
```

This is now recognizably:

> a primitive web server architecture

---

# 🧩 Relation to Final Repo

This phase aligns heavily with:

* response generation
* asset serving
* file handling logic

Still missing:

* Request abstraction
* Response abstraction
* Server class

Those are the next architectural leap.

---

# ✅ Phase 5 Checkpoint

You are done when:

* [ ] CSS loads correctly
* [ ] DevTools shows correct MIME types
* [ ] Browser requests multiple assets
* [ ] curl confirms MIME headers
* [ ] You understand:

  * MIME purpose
  * browser asset loading behavior
  * extension-based content negotiation

---

# 🧭 Next Phase Preview

## Phase 6 — Request Parsing Abstraction

We now move from:

```cpp
raw buffer handling
```

to:

```cpp
Request object abstraction
```

This is where your codebase begins transitioning from:

> “single-file experiment”

into:

> “maintainable server architecture”

And it maps directly toward:

* `request.cpp`
* `request.h`

in your final repo.
