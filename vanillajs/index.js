import { getPageFromUrl } from "./utils/page-routing.js";
import { setupNavigation } from "./utils/navigation.js";
import { prefetchPageTemplates, renderPageContent } from "./utils/template-service.js";

window.addEventListener("DOMContentLoaded", async () => {
    const mainContainer = document.querySelector("main");
    if (!mainContainer) {
        return;
    }

    setupNavigation(mainContainer, renderPageContent);

    await renderPageContent(mainContainer, getPageFromUrl(), { pushHistory: false });
    prefetchPageTemplates();
});