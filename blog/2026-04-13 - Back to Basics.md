# 1. Introduction
In todays environment there are a bunch of frameworks, modules, services and AI that can help set up apps and websites in a heart beat and that is awesome. Personally I've been working years with Node, React and Next and looked into a few other frameworks like Astro, Vue and Solid. I've used Webpack, Express.js or simply the http package to play with web servers but I never really attempted to look into what is behind the curtain.

I imagine many before me has taken a similar path. As a beginner you learn some basics like variables, iterating an array etc. Then you get into frameworks and everything gets abstracted so much but you learn to work within that framework and don't ask too many questions as to why and how Next handles routes, how Express serves files and handles requests and more.
As I'm building my own portfolio page I want to test out how to handle these things myself: namely create a vanilla js SPA, serving the files myself on my own server created in C++ and set up my own very rudimentary SQL database. However I know that I won't be able to match even closely the already production ready modules and frameworks both in efficiency, scalability or most importantly security. Especially dappling in server architecture I will mostly build this as a way to learn and work on it locally. Therefore the actual deployment of my portfolio to be, will be handled using an actual framework such as Express at least until I reach a level of security on my own server achitecture (TLS/SSL, handling multiple clients, HTTP/1.1 and more). The same can be said for a database but I have yet to decide upon an approach for this.

# 2. The Motivation and Goal
I want to learn how to build a portfolio as a simple app both front end and back end but relying on as few modules at all. I want to look into creating my own vanilla js framework, handling styling etc without relying on express, tailwind etc. I want to handle routing, requests, auth, database and all that stuff. I want to document my whole process, how, when and why I use AI tools and ultimately check those skills off as part of my portfolio. Later I want to Portfolio page to show case my own projects but also my educational and work experience, while still serving as a blog showing my progress as a developer and use of tools. I also intend to dapple with numerous frameworks recreating similar style full stack apps in Next, Nuxt, Angular, Vue, Astro etc. and dapple with CMS like Wordpress, Shopify, Wix, Contenful, Strapi, Sanity and perhaps even my own custom CMS. I may also include some simple games using web sockets and/or TCP and a DB. 

In the end this portfolio page should include the progress of building it, actual work expertise, projects I've made and worked on and some games/tools made primarily with Unity. 

# 3. The Current State of Development
Currently the whole project is in its initial setup phases. The repo has been setup, a very simple webpage and an initial server has been created. Then the documentation of the use of AI tools, for learning purposes and organisation has been added as learning-resources and an initial blog system is handled using markdown for now. May change later, but the initial idea is to handle markdown files and have them served on the webpage using a markdown parser I will create later.

For now I'm building a C++ server in phases as part of a tutorial co-piloted by ChatGPT, Mistral AI (for comparison) and GitHub Copilot when working in vscode. I am documenting the progress, writing this blog and preparing the next phase of learning to create a webserver in C++.

# 4. Discussion on Phase 0 and 1
I looked up how to build your own web server as wanted to try and not rely on express or even the http package within my vanilla js app in order to serve my http files. I found an interesting [Medium article](https://medium.com/@aryandev512/i-wrote-a-http-server-from-scratch-in-c-0a97e8252371) and its [GitHub repository](https://github.com/Aryandev12/webby-http-server/tree/main) which laid the base for my investigation of the topic. I chose to feed the article and the repository to Chat GPT and ask it to use it as a base to construct a university level type course that looked into building a Web Server from scratch using the article as a base for information but the repo as its final state of the course. Then split the course into phases where we learn basics while integrating basic features one feature at the time. E.g before even going into sockets I refresh my memory on the OSI model and some basic Data Communications

# 5. Tools of choice
- **OS:** macOS (Apple M1)
- **IDE:** Visual Studio Code (vscode)
- **Browser:** Firefox/Chrome
- **Terminal:** Warp or iTerm
- **Version Control and CI/CD:** GitHub using cli.
- **Documentation:** Obsidian (markdown)
# 5. Up Next 
Next I want to I want to Read Data on the server and document that along with looking into how to integrate the blog on the web page and parsing the markdown.