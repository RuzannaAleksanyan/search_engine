// Opens the settings modal on the personal information page
function openSettingsModalPersonalInformationPage() {
  closeLanguageModal();

  const modal = document.getElementById("settings-modal");
  modal.style.display = "block";
}

function openLanguagePagePersonalInformationModal() {
  closeSettingsModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}
