import { getPageFromUrl, pageNames } from "./page-routing.js";
/**
 * Sets up navigation by intercepting clicks on links with "?page=" and handling browser back/forward buttons.
 * @param {HTMLElement} mainContainer - The main container where page content will be rendered.
 * @param {(mainContainer: HTMLElement, pageName: string, options?: { pushHistory: boolean }) => Promise<void>} renderPageContent - The function to call for rendering page content.
 */
export const setupNavigation = (mainContainer, renderPageContent) => {
    document.addEventListener("click", (event) => {
        const navLink = event.target.closest('a[href*="?page="]');
        if (!navLink) {
            return;
        }

        const targetUrl = new URL(navLink.href, window.location.href);
        if (targetUrl.origin !== window.location.origin) {
            return;
        }

        const targetPageName = targetUrl.searchParams.get("page");
        if (!targetPageName || !pageNames.includes(targetPageName)) {
            return;
        }

        event.preventDefault();
        void renderPageContent(mainContainer, targetPageName, { pushHistory: true });
    });

    window.addEventListener("popstate", () => {
        void renderPageContent(mainContainer, getPageFromUrl(), { pushHistory: false });
    });
};
