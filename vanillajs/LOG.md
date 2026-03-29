#Project Log

## 001 - First iteration of SPA
Setting up a simple SPA with a header, a few pages and injecting a main element with html from templates with performance in mind. Wrote a bit on the README

### Naming/API considerations
- Kept module responsibilities separated: routing, navigation wiring, and template service.
- Standardized function names using `verb + object + context` to make intent obvious in imports and call sites.
- Adopted these API names:
	- `fetchPageTemplate` (internal template fetch helper)
	- `renderPageContent` (rendering into main container)
	- `prefetchPageTemplates` (background loading)
- Kept parameter names consistent across modules (`pageName`, `mainContainer`).

### Performance considerations for future content growth
- Keep initial route render prioritized; defer broad prefetch work to idle time.
- Treat images as primary performance budget item: use modern formats, explicit dimensions, and lazy loading for non-critical images.
- Keep critical path small: minimal render-blocking CSS/JS and only essential assets loaded for first paint.
- Use caching/compression strategy for static assets and templates to reduce repeated transfer cost.
- Maintain lightweight startup work in `DOMContentLoaded` and move non-essential tasks off the critical path.

### AI/Copilot co-authoring disclosure
- GitHub Copilot was used as a coding assistant for refactoring ideas, naming consistency, and documentation drafting.
- Suggestions were reviewed and adjusted before being kept.
- Final implementation were made by the project me.

### Safety and error logging pass
Added a final safety-oriented pass to improve error visibility and make function behavior easier to diagnose.

- Added a shared logger utility with standardized prefix and log helpers (`logInfo`, `logWarning`, `logError`).
- Added defensive logging in bootstrap flow, navigation handlers, routing validation, rendering, and template prefetch paths.
- Added guarded handling for unexpected failures in history updates and async render/prefetch execution.
- Added JSDoc function comments for all logger functions.
- This safety/error-logging pass was co-authored with GitHub Copilot.

