import { logWarning } from "./logger.js";

/**
 * An array of valid page names.
 */
export const pageNames = ["home", "projects", "about", "contact"];

/**
 * Validates the given page name and returns a valid page name.
 * @param {string} pageName - The name of the page to validate.
 * @returns {string} The valid page name.
 */
export const getValidPageName = (pageName) => {
    if (pageName && !pageNames.includes(pageName)) {
        logWarning("Invalid page name in URL. Falling back to home.", { pageName });
    }

    return pageNames.includes(pageName) ? pageName : "home";
};

/**
 * Gets the current page name from the URL.
 * @returns {string} The current page name.
 */
export const getPageFromUrl = () => {
    const urlParams = new URLSearchParams(window.location.search);
    return getValidPageName(urlParams.get("page"));
};
