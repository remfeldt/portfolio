import { getPageFromUrl, getValidPageName, pageNames } from "./page-routing.js";

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
            console.error(error);
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
    const validPageName = getValidPageName(pageName);

    if (!templateCache.has(validPageName)) {
        mainContainer.innerHTML = "<p>Loading...</p>";
    }

    const htmlTemplate = await fetchPageTemplate(validPageName);
    mainContainer.innerHTML = htmlTemplate;

    if (options.pushHistory) {
        window.history.pushState({ page: validPageName }, "", `?page=${validPageName}`);
    }
};

/**
 * Prefetches HTML templates for all pages except the current one.
 */
export const prefetchPageTemplates = () => {
    const currentPage = getPageFromUrl();
    const pagesToPrefetch = pageNames.filter((pageName) => pageName !== currentPage);
    const runPrefetch = () => {
        pagesToPrefetch.forEach((pageName) => {
            void fetchPageTemplate(pageName);
        });
    };

    if ("requestIdleCallback" in window) {
        window.requestIdleCallback(runPrefetch);
    } else {
        window.setTimeout(runPrefetch, 0);
    }
};
