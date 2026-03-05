export const pageNames = ["home", "projects", "about", "contact"];

export const getValidPageName = (pageName) => {
    return pageNames.includes(pageName) ? pageName : "home";
};

export const getPageFromUrl = () => {
    const urlParams = new URLSearchParams(window.location.search);
    return getValidPageName(urlParams.get("page"));
};
