function openSettingsModalUI() {
  closeLanguageModal();

  const modal = document.getElementById("settings-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openLanguagePageModalUI() {
  closeSettingsModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function showHideUI() {
  var passInput = document.getElementById('pass');

  if (passInput.type === 'password') {
    passInput.type = 'text';
  } else {
    passInput.type = 'password';
  }

  // Toggle the visibility of the eye icons
  var hideIcon = document.querySelector('.hide');
  var showIcon = document.querySelector('.show');

  if (passInput.type === 'password') {
    hideIcon.style.display = 'block';
    showIcon.style.display = 'none';
  } else {
    hideIcon.style.display = 'none';
    showIcon.style.display = 'block';
  }
}
