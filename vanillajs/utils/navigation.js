import { getPageFromUrl, pageNames } from "./page-routing.js";

export const setupNavigation = (mainContainer, renderPage) => {
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
        void renderPage(mainContainer, targetPageName, { pushHistory: true });
    });

    window.addEventListener("popstate", () => {
        void renderPage(mainContainer, getPageFromUrl(), { pushHistory: false });
    });
};
