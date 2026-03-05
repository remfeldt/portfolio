import { getPageFromUrl } from "./utils/page-routing.js";
import { setupNavigation } from "./utils/navigation.js";
import { logError, logInfo } from "./utils/logger.js";
import { prefetchPageTemplates, renderPageContent } from "./utils/template-service.js";

window.addEventListener("DOMContentLoaded", async () => {
    const mainContainer = document.querySelector("main");
    if (!mainContainer) {
        logError("Application bootstrap aborted because <main> was not found.");
        return;
    }

    try {
        setupNavigation(mainContainer, renderPageContent);
        await renderPageContent(mainContainer, getPageFromUrl(), { pushHistory: false });
        prefetchPageTemplates();
        logInfo("Welcome to my portfolio!");
    } catch (error) {
        logError("Unexpected error during application bootstrap.", error);
    }
});