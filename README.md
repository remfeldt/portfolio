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
	- Includes the initial prompt, Phase 0-8 writeups, and supporting reference material.
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

Based on the Phase 9 checkpoint, the project is around halfway to the reference end state.

```txt
[############--------]
60%
```

Short phase list:

1. Foundations and TCP socket basics
2. Receiving and sending HTTP over raw sockets
3. Static file serving and MIME handling
4. Request abstraction (`Request` parser layer)
5. Response abstraction (`Response` serializer layer)
6. Server abstraction (`Server` class)
7. Next: server decomposition, routing, utility cleanup, and repo convergence

## Current Status

The hard protocol and networking concepts are in place. Remaining work is primarily architectural refinement: cleaner server decomposition, improved routing flow, stronger organization, and final alignment with the reference repository structure.
