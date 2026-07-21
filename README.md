# Portfolio + Web Server From First Principles

This repository documents a learning-first journey: build a C++ HTTP server from scratch, understand each systems-level decision, and use it to serve a barebones HTML/CSS/vanilla JavaScript portfolio. The project is guided by a university-style phase model focused on implementation details, function-level reasoning, and architecture tradeoffs. The long-term direction is to later expose selected C++ server functionality to JavaScript as a custom module.

## Project Structure

Current repository layout:

```txt
LICENSE
README.md
blog/
documentation/
learning-resources/
server/
vanillajs/
```

Important folders:

- `server/`
	- Core C++ web server implementation.
	- `src/` contains `main.cpp`, `server.cpp/.h`, `request.cpp/.h`, `response.cpp/.h`, and static test assets (`index.html`, `styles.css`).
	- This is the main implementation target for the tutorial phases.
- `learning-resources/`
	- Course architecture and phased notes used to drive development.
	- Includes the initial prompt, Phase 0-11 writeups, and supporting reference material.
- `vanillajs/`
	- Frontend portfolio prototype built with plain HTML, CSS, and JavaScript.
	- Intended to be served by the custom C++ server as the project matures.
- `templates/`
	- Reusable HTML templates for pages like home, about, projects, and contact.
- `utils/`
	- JavaScript helpers for logging, navigation, routing, and template loading.
- `blog/`
	- Development journal and learning notes.
- `documentation/`
	- Additional project documentation and reference material.

## Phase Progress (Current)

After Phase 11 completion, the project is around 70% toward the reference end state.

```txt
[###############-----] 75%
```

### Completion Status

- Foundation & Networking       ✅ 100%
- HTTP Protocol Basics          ✅ 100%
- Request Parsing               ✅ 100%
- Response Generation           ✅ 100%
- Static File Serving           ✅ 100%
- MIME Handling                 ✅ 100%
- Server Abstraction            ✅ 100%
- Server Decomposition          ✅ 100%

- Routing Layer                 ✅ 100% (Phase 10)
- Request Dispatching           ✅ 100% (Phase 10)
- Route Table Architecture      ✅ 100% (Phase 11)
- Repository Structure Match    ⏳ 60%
- Response Helpers              ✅ 100% (Phase 12)
- Polish & Refinement           ⏳ 0%

### Phase Summary

- **Phase 1–8:** Core networking and HTTP protocol implementation
- **Phase 9:** Server decomposition into cohesive methods (`handleClient()`, `serveFile()`, `getMimeType()`)
- **Phase 10:** Routing and request dispatch — introducing request-based handlers, dynamic endpoints, and request-processing pipeline
- **Phase 11:** Route table architecture (`std::unordered_map<std::string, std::function<void(int)>>`) for scalable handler registration
- **Phase 12+:** Response helpers, utility extraction, and final repository convergence

## Current Status

The server now supports table-driven routing with registered handlers, moving beyond conditional route checks to a scalable dispatch architecture. The project has evolved from a pure static file server into a mini web framework that can serve both files and dynamic endpoints. Remaining work focuses on response helper utilities, repository alignment, and final polish.