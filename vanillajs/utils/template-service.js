import { getPageFromUrl, getValidPageName, pageNames } from "./page-routing.js";
import { logError, logWarning } from "./logger.js";

const templateCache = new Map();
const inFlightRequests = new Map();

/**
 * Fetches the HTML template for a given page.
 * @param {string} pageName - The name of the page to fetch the template for.
 * @returns {Promise<string>} A promise that resolves to the HTML template.
 */
const fetchPageTemplate = (pageName) => {
    const validPageName = getValidPageName(pageName);

    if (templateCache.has(validPageName)) {
        return Promise.resolve(templateCache.get(validPageName));
    }

    if (inFlightRequests.has(validPageName)) {
        return inFlightRequests.get(validPageName);
    }

    const request = fetch(`templates/${validPageName}.html`)
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Failed to fetch template for ${validPageName}`);
            }

            return response.text();
        })
        .then((htmlTemplate) => {
            templateCache.set(validPageName, htmlTemplate);
            return htmlTemplate;
        })
        .catch((error) => {
            logError("Template fetch failed.", { pageName: validPageName, error });
            return "<p>Sorry, this section is unavailable right now.</p>";
        })
        .finally(() => {
            inFlightRequests.delete(validPageName);
        });

    inFlightRequests.set(validPageName, request);
    return request;
};

/**
 * Renders a page into the main container.
 * @param {HTMLElement} mainContainer - The main container where page content will be rendered.
 * @param {string} pageName - The name of the page to render.
 * @param {Object} options - Options for rendering the page.
 * @param {boolean} options.pushHistory - Whether to push the page to the browser history.
 */
export const renderPageContent = async (mainContainer, pageName, options = { pushHistory: true }) => {
    if (!mainContainer) {
        logError("Render skipped because main container is missing.");
        return;
    }

    const validPageName = getValidPageName(pageName);

    if (!templateCache.has(validPageName)) {
        mainContainer.innerHTML = "<p>Loading...</p>";
    }

    try {
        const htmlTemplate = await fetchPageTemplate(validPageName);
        mainContainer.innerHTML = htmlTemplate;
    } catch (error) {
        logError("Unexpected error while rendering page content.", {
            pageName: validPageName,
            error
        });
        mainContainer.innerHTML = "<p>Sorry, this section is unavailable right now.</p>";
    }

    if (options.pushHistory) {
        try {
            window.history.pushState({ page: validPageName }, "", `?page=${validPageName}`);
        } catch (error) {
            logError("Failed to update browser history state.", {
                pageName: validPageName,
                error
            });
        }
    }
};

/**
 * Prefetches HTML templates for all pages except the current one.
 */
export const prefetchPageTemplates = () => {
    const currentPage = getPageFromUrl();
    const pagesToPrefetch = pageNames.filter((pageName) => pageName !== currentPage);
    const runPrefetch = () => {
        try {
            pagesToPrefetch.forEach((pageName) => {
                void fetchPageTemplate(pageName);
            });
        } catch (error) {
            logError("Unexpected error during template prefetch loop.", error);
        }
    };

    if ("requestIdleCallback" in window) {
        try {
            window.requestIdleCallback(runPrefetch);
        } catch (error) {
            logWarning("requestIdleCallback failed. Falling back to setTimeout prefetch.", error);
            window.setTimeout(runPrefetch, 0);
        }
    } else {
        window.setTimeout(runPrefetch, 0);
    }
};
