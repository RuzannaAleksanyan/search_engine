document.addEventListener("DOMContentLoaded", function () {
  var infoParagraphs = document.querySelectorAll(
    "#info > dev:not(.hidden) > p"
  );
  infoParagraphs.forEach(function (paragraph) {
    paragraph.style.transition = "transform 0.5s ease-in-out";
    paragraph.style.transform = "translateX(0)";
  });
});

// Changes the language selector and updates displayed information accordingly
function changeLanguageSelector() {
  var languageSelector = document.getElementById("language");
  var selectedLanguage = languageSelector.value;

  var languageInfoElements = document.querySelectorAll("#info > dev");
  console.log(languageInfoElements);
  languageInfoElements.forEach(function (element) {
    if (element.id === selectedLanguage) {
      element.classList.remove("hidden");
    } else {
      element.classList.add("hidden");
    }
  });
  var paragraphs = document.querySelectorAll("#info > dev:not(.hidden) > p");
  paragraphs.forEach(function (paragraph) {
    paragraph.style.transition = "transform 0.5s ease-in-out";
    if (paragraph.parentElement.id === selectedLanguage) {
      paragraph.style.transform = "translateX(0)";
    } else {
      paragraph.style.transform = "translateX(100%)";
    }
  });

  // translateContent(languageSelector);
}

// Opens the login modal on the language page
function openLoginModalLanguagePage() {
  let modal = document.getElementById("login-page-modal");
  let modals = document.getElementById("modals");

  modal.style.display = "block";
  modals.style.display = "block";
}

// Opens the settings modal on the language page
function openSettingsModalLanguagePage() {
  let modals = document.getElementById("modals");
  const modal = document.getElementById("settings-modal");
  modals.style.display = "none";
  modal.style.display = "block";
}
