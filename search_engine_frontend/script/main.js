function openLoginModalMainPage() {
  closeRegistrationModal();

  let modal = document.getElementById("login-page-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function closeAllModalsMainPage(event) {
  // Get cursor type from mousemove event
  document.addEventListener("click", function (mouseEvent) {
    const cursorType = window.getComputedStyle(mouseEvent.target).cursor;

    // Check if cursor type is not pointer and close all modals
    if (
      cursorType !== "pointer" &&
      cursorType !== "text" &&
      cursorType !== "move"
    ) {
      closeModalsMainPage();
    }
  });
}

// Add event listener to document for click event
document.addEventListener("click", closeAllModalsMainPage);

// Function to close all modals
function closeModalsMainPage() {
  closeLanguageModal();
  closeLoginModal();
  closeRegistrationModal();
  closeFrogotPassword();
  closeChangePassword();
}

function openLanguageModalManPage() {
  closeLoginModal();
  closeRegistrationModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}
