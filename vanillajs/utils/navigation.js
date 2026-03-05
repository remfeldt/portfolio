import { getPageFromUrl, pageNames } from "./page-routing.js";
import { logError, logWarning } from "./logger.js";
/**
 * Sets up navigation by intercepting clicks on links with "?page=" and handling browser back/forward buttons.
 * @param {HTMLElement} mainContainer - The main container where page content will be rendered.
 * @param {(mainContainer: HTMLElement, pageName: string, options?: { pushHistory: boolean }) => Promise<void>} renderPageContent - The function to call for rendering page content.
 */
export const setupNavigation = (mainContainer, renderPageContent) => {
    if (!mainContainer) {
        logError("Navigation setup skipped because main container is missing.");
        return;
    }

    if (typeof renderPageContent !== "function") {
        logError("Navigation setup skipped because render callback is invalid.", {
            renderPageContent
        });
        return;
    }

    document.addEventListener("click", (event) => {
        const navLink = event.target.closest('a[href*="?page="]');
        if (!navLink) {
            return;
        }

        const targetUrl = new URL(navLink.href, window.location.href);
        if (targetUrl.origin !== window.location.origin) {
            logWarning("Blocked cross-origin navigation attempt.", { href: navLink.href });
            return;
        }

        const targetPageName = targetUrl.searchParams.get("page");
        if (!targetPageName || !pageNames.includes(targetPageName)) {
            logWarning("Blocked navigation with invalid page parameter.", {
                targetPageName,
                href: navLink.href
            });
            return;
        }

        event.preventDefault();
        void renderPageContent(mainContainer, targetPageName, { pushHistory: true }).catch((error) => {
            logError("Failed to render target page after navigation click.", {
                targetPageName,
                error
            });
        });
    });

    window.addEventListener("popstate", () => {
        void renderPageContent(mainContainer, getPageFromUrl(), { pushHistory: false }).catch((error) => {
            logError("Failed to render page on popstate navigation.", error);
        });
    });
};
