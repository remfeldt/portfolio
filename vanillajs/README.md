# Vanilla JS Portfolio by Alexander P. Remfeldt

## Introduction
There are many awesome frameworks, node modules and libraries and they keep advanced implementations like routing, performance and project structure at a high abstraction level, which is nice when you want to create content fast. It locks the user into predetermined rules, constraints and version maintenance. The user don't have to worry about "How do routing actually work?". Just create a directory in the pages/app folder (next.js e.g) and it works out the gate. 

It is all nice and handy, but having worked many years with frameworks, node modules and other libraries I wanted to go on my own vanilla JS adventure, as many have done before me. I want to build a simple SPA with the following in mind:
- Project structure
- Performance
- Semantics and Accessibility
- Fun

The intention of this project is to be worked on iterarively as I learn and improve in which the project will also improve. Therefore I will try and document each iteration in a ([Project Log](/LOG.md)), what I wanted to change, what actually changed, what I learned, link references/sources etc. It's important to me that I have fun while doing this, so I may have set up some initial goals but along the way I may feel the urge to add additional content as I'm building. I will log this as well.

I work in VS Code with the Live Server extension, git and github cli in the terminal. Keeping this project nice and small I will have everything happen clientside but I will look into building a server at a later time, to test and play with server side rendering, auth, db integrations and more. 

## Function Naming Convention
To keep APIs consistent and readable as the project grows, this project uses a `verb + object + context` naming style.

- `setup*` for side-effect wiring and event listeners (example: `setupNavigation`)
- `get*` for pure reads without side effects (example: `getPageFromUrl`)
- `render*` for DOM output/injection (example: `renderPageContent`)
- `prefetch*` for background loading (example: `prefetchPageTemplates`)
- Internal fetch helpers should stay concise and explicit (example: `fetchPageTemplate`)

Parameter naming should stay consistent across modules:
- `pageName` for route/template identifiers
- `mainContainer` for the `<main>` render target

## Co-authoring with GitHub Copilot
Parts of this project were co-authored with GitHub Copilot in VS Code. Copilot was used as a coding assistant for:

- Refactoring and naming suggestions
- Drafting utility-module structure and JSDoc comments
- Reviewing readability and consistency in function APIs
- Iterating on implementation ideas and tradeoffs

All generated suggestions were reviewed, adapted/adjusted, and validated before being kept in the codebase. Final design and implementation decisions remain with the project me.

