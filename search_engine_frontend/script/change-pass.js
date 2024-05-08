function openSettingsModalChangePass() {
  // closeLanguageModal();

  const modal = document.getElementById("settings-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openLanguageModalChangePass() {
  closeSettingsModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}