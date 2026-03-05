import { getPageFromUrl } from "./utils/page-routing.js";
import { setupNavigation } from "./utils/navigation.js";
import { prefetchTemplates, renderPage } from "./utils/template-service.js";

window.addEventListener("DOMContentLoaded", async () => {
    const mainContainer = document.querySelector("main");
    if (!mainContainer) {
        return;
    }

    setupNavigation(mainContainer, renderPage);

    await renderPage(mainContainer, getPageFromUrl(), { pushHistory: false });
    prefetchTemplates();
});