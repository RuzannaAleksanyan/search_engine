// Opens the settings modal and hides other modals
function openSettingsHistoryModal() {
  closeLoginModal();
  closeRegistrationModal();
  closeLanguageModal();

  let modal = document.getElementById("settings-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

// Opens the login history modal and displays it
function openLoginHistoryModal() {
  closeRegistrationModal();
  closeSettingsModal();
  closeLanguageModal();

  const modal = document.getElementById("login-page-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

// Opens the language modal and hides other modals
function openLanguagePageHistoryModal() {
  closeSettingsModal();
  closeLoginModal();
  closeRegistrationModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}
