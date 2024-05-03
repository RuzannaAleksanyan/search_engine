// Function to fetch language data
async function fetchLanguageData(lang) {
    // const response = await fetch(`languages/${lang}.json`);
    // return response.json();
    let json = JSON.parse(sessionStorage.getItem(`languages/${lang}`));

    if (!json) {
        try {
            const response = await fetch(`languages/${lang}.json`);
            if (!response.ok) {
                throw new Error(`Failed to fetch language data for ${lang}`);
            }

            json = await response.json();
            sessionStorage.setItem(`languages/${lang}`, JSON.stringify(json));
        } catch (error) {
            console.error(error);
            throw error;
        }
    }

    return json;
}

// Function to set the language preference
function setLanguagePreference(lang) {
    localStorage.setItem("language", lang);
    location.reload();
}


function openLanguagePageModal() {
    const modal = document.getElementById("language-page");
    if (modal.style.display === "block") {
        modal.style.display = "none";
    } else {
        modal.style.display = "block";
    }
}

// Function to update content based on selected language
function updateContent(langData) {
    document.querySelectorAll("[data-i18n]").forEach((element) => {
        const keys = element.getAttribute("data-i18n").split("."); // Split keys by dot
        let value = langData;
        keys.forEach((key) => {
            value = value[key]; // Traverse through nested keys
        });
        element.textContent = value;
    });
    document.querySelectorAll('input[data-i18n]').forEach((input, index) => {
        const keys = input.getAttribute("data-i18n").split("."); // Split keys by dot
        let value = langData;
        keys.forEach((key) => {
            value = value[key]; // Traverse through nested keys
        });
        input.setAttribute("placeholder", value);
    });
}

// Function to change language
async function changeLanguage(lang) {
    setLanguagePreference(lang);
    const langData = await fetchLanguageData(lang);

    updateContent(langData);
}

// // Call updateContent() on page load
window.addEventListener("DOMContentLoaded", async () => {
    if (!localStorage.getItem("language")) localStorage.setItem("language", "am");
    const userPreferredLanguage = localStorage.getItem("language") || "am";
    const langData = await fetchLanguageData(userPreferredLanguage);
    updateContent(langData);
});