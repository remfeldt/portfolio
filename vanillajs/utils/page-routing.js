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
