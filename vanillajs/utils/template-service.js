import { getPageFromUrl, getValidPageName, pageNames } from "./page-routing.js";

const templateCache = new Map();
const inFlightRequests = new Map();

const fetchTemplate = (pageName) => {
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

export const renderPage = async (mainContainer, pageName, options = { pushHistory: true }) => {
    const validPageName = getValidPageName(pageName);

    if (!templateCache.has(validPageName)) {
        mainContainer.innerHTML = "<p>Loading...</p>";
    }

    const htmlTemplate = await fetchTemplate(validPageName);
    mainContainer.innerHTML = htmlTemplate;

    if (options.pushHistory) {
        window.history.pushState({ page: validPageName }, "", `?page=${validPageName}`);
    }
};

export const prefetchTemplates = () => {
    const currentPage = getPageFromUrl();
    const pagesToPrefetch = pageNames.filter((pageName) => pageName !== currentPage);
    const runPrefetch = () => {
        pagesToPrefetch.forEach((pageName) => {
            void fetchTemplate(pageName);
        });
    };

    if ("requestIdleCallback" in window) {
        window.requestIdleCallback(runPrefetch);
    } else {
        window.setTimeout(runPrefetch, 0);
    }
};
