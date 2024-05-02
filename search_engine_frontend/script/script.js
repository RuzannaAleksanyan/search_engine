// localStorage.clear();
document.addEventListener("DOMContentLoaded", function () {
  const searchInput = document.getElementById("search-input");
  const urlParams = new URLSearchParams(window.location.search);

  const pageHeader = document.getElementById("page-header");

  if(searchInput) {
    searchInput.addEventListener("keydown", function (event) {
      if (event.key === "Enter" && searchInput.value.trim() !== "") {
        window.location.href = `second_page.html?searchTerm=${searchInput.value.trim()}`
        
        // const searchTerms = searchInput.value.split(' ');
        // console.log(searchTerms);
        
        // searchInput.value = "";
        
        // fetch("http://localhost:9999/api/search", {
        //   method: "POST",
        //   headers: {
        //     "Content-Type": "application/json",
        //   },
        //   body: JSON.stringify({ searchTerms }),
        // })
        // .then(f => f.json())
        // .then((data) => {
        //   if (data.data !== undefined) {
        //     console.log("hello");
        //     // if(res.length >= 1) {
        //     //   displayURLs(res);
        //     // } else {
        //     //   searchResultNotFound();
        //     // }
        //   } else {
        //     console.log('Invalid data format received.');
        //   }
        // });
      }
    });
  }

  window.addEventListener("beforeunload", function (event) {
    if (!enterPressed) {
      // Simulate Ctrl+R key press
      if (event.ctrlKey || event.metaKey) {
        window.location.reload();
      }
    }
  });
});

// Function to close all modals when clicking outside of them
function closeAllModals(event) {
  // Get cursor type from mousemove event
  document.addEventListener("click", function (mouseEvent) {
    const cursorType = window.getComputedStyle(mouseEvent.target).cursor;

    // Check if cursor type is not pointer and close all modals
    if (
      cursorType !== "pointer" &&
      cursorType !== "text" &&
      cursorType !== "move"
    ) {
      closeModals();
    }
  });
}

// Add event listener to document for click event
document.addEventListener("click", closeAllModals);

// Function to close all modals
function closeModals() {
  closeLanguageModal();
  closeSettingsModal();
  closeLoginModal();
  closeRegistrationModal();
  closeAccountModal();
  closeShortcutsModal();
  closeAddedShortcutsPageModal();
  closeFrogotPassword();
  closeChangePassword();
}

function openSearchEnginePage() {
  window.location.href = "index.html";
}

function openSettingsModal() {
  closeLoginModal();
  closeRegistrationModal();
  closeShortcutsModal();
  closeAccountModal();
  closeLanguageModal();

  const modal = document.getElementById("settings-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openLoginModal() {
  closeRegistrationModal();
  closeShortcutsModal();
  closeAccountModal();
  closeLanguageModal();
  closeSettingsModal();

  let modal = document.getElementById("login-page-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function closeLoginModal() {
  let modal = document.getElementById("login-page-modal");
  modal.style.display = "none";
}

function openRegistrationModal() {
  closeLoginModal();

  let modal = document.getElementById("registration-page-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function closeRegistrationModal() {
  document
    .getElementById("registration-page-modal")
    .querySelector(".modal-content-registration-page")
    .classList.remove("show");
  document.getElementById("registration-page-modal").style.display = "none";
}

function openAccountModal() {
  closeRegistrationModal();
  closeShortcutsModal();
  closeLoginModal();
  closeLanguageModal();
  closeSettingsModal();

  let modal = document.getElementById("account-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openShortcutsModal() {
  closeRegistrationModal();
  closeAccountModal();
  closeLoginModal();
  closeLanguageModal();
  closeSettingsModal();

  const modal = document.getElementById("shortcuts-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function closeSettingsModal() {
  const modal = document.getElementById("settings-modal");
  modal.style.display = "none";
}

function openHistoryPage() {
  window.location.href = "history.html";
}

function openLanguagePageModal() {
  closeSettingsModal();
  closeAccountModal();
  closeLoginModal();
  closeRegistrationModal();
  closeShortcutsModal();

  const modal = document.getElementById("language-page");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openPersonalInfoPageModal() {
  window.location.href = "personal-information.html";
}

document.addEventListener("DOMContentLoaded", function () {
  const savedTheme = localStorage.getItem("theme");
  if (!savedTheme || savedTheme === "light") {
    document.documentElement.setAttribute("data-theme", "light");
  } else {
    document.documentElement.setAttribute("data-theme", savedTheme);
  }
});

function changeBackground() {
  var root = document.documentElement;
  var selectedTheme = localStorage.getItem("theme");

  if (selectedTheme == "light") {
    root.setAttribute("data-theme", "dark");
    localStorage.setItem("theme", "dark"); // Store selected theme in localStorage
  } else {
    root.setAttribute("data-theme", "light");
    localStorage.setItem("theme", "light"); // Store selected theme in localStorage
  }
}

function closeLanguageModal() {
  const modal = document.getElementById("language-page");
  modal.style.display = "";
}

function closeShortcutsModal() {
  const modal = document.getElementById("shortcuts-modal");
  modal.style.display = "";
}

var index = 0;

// // Load saved shortcuts when the page loads
// window.addEventListener('DOMContentLoaded', function () {
//   for (let i = 0; i < localStorage.length; i++) {
//     let key = localStorage.key(i);
//     if (key.startsWith('shortcut')) {
//       let shortcut = JSON.parse(localStorage.getItem(key));
//       let elements = document.getElementsByClassName("shrt-button");
//       let hoverMessages = document.getElementsByClassName("shrt-text");
//       let message = hoverMessages[index];
//       let element = elements[index];
//       element.textContent = shortcut.label[0];
//       message.textContent = shortcut.label;
//       message.style.display = 'block';
//       element.addEventListener("click", function () {
//         closeAddedShortcutsPageModal();
//         window.location.href = shortcut.url;
//       });
//       ++index;
//     }
//   }
// });

function updatingDB(url, name) {
  console.log(userEmail);
  
}

function addedShortcut() {
  let utl = document.getElementById("link-url-input");
  let label = document.getElementById("link-label-input");
  let errorMessage1 = document.getElementById("error-message1");
  let errorMessage2 = document.getElementById("error-message2");
  let url = utl.value.trim();
  let name = label.value.trim();
  errorMessage1.textContent = "";
  errorMessage2.textContent = "";
  let lang = localStorage.getItem("selectedLanguage") || "arm";
  if (url === "") {
    let translations = {
      arm: "Խնդրում ենք մուտքագրել URL",
      en: "Please enter URL",
      ru: "Пожалуйста, введите URL",
    };
    errorMessage1.textContent = translations[lang];
    utl.focus();
    return;
  }
  if (name === "") {
    let translations = {
      arm: "Խնդրում ենք մուտքագրել պիտակ",
      en: "Please enter Label",
      ru: "Пожалуйста, введите метку",
    };
    errorMessage2.textContent = translations[lang];
    label.focus();
    return;
  }

  let button_name = document.getElementById("link-label-input");
  let elements = document.getElementsByClassName("shrt-button");
  let hoverMessages = document.getElementsByClassName("shrt-text");
  let message = hoverMessages[index];
  let element = elements[index];
  element.textContent = button_name.value[0];

  element.addEventListener("click", function () {
    closeAddedShortcutsPageModal();
    window.location.href = url; // Navigate to the URL specified in the input field
  });

  message.textContent = button_name.value;
  message.style.display = "block";

  // Save the added shortcut in localStorage
  localStorage.setItem('shortcut' + index, JSON.stringify({ url: url, label: button_name.value }));

  updatingDB(url, name);

  ++index;
  closeAddedShortcutsPageModal();
}

function closeAddedShortcutsPageModal() {
  document
    .getElementById("added-shortcuts-page-modal")
    .querySelector(".modal-content-added-shortcuts-page")
    .classList.remove("show");
  document.getElementById("added-shortcuts-page-modal").style.display = "none";
}

function closeAccountModal() {
  const modal = document.getElementById("account-modal");
  modal.style.display = "none";
}

function translateContent(lang) {
  let elements = document.querySelectorAll("[data-lang]");
  let buttons = document.querySelectorAll(
    ".modal-content-login-page button, .modal-content-added-shortcuts-page button, .modal-content-delete-shortcuts-page button, .modal-content-settings button"
  );
  let searchInput = document.getElementById("search-input");
  let inputFields = document.querySelectorAll(".input, .input-container input");
  let forgotPasswordLink = document.querySelector(".remember-forgot a");
  let showPassword = document.querySelector(".show");
  let errorMessages = document.querySelectorAll(".error-message");

  // Save selected language to localStorage
  localStorage.setItem("selectedLanguage", lang);

  inputFields.forEach(function (inputField) {
    inputField.value = "";
  });

  errorMessages.forEach(function (errorMessage) {
    errorMessage.textContent = "";
  });

  elements.forEach(function (element) {
    if (element.dataset.lang) {
      let translations = JSON.parse(element.dataset.lang);
      if (translations.hasOwnProperty(lang)) {
        element.textContent = translations[lang];
      }
    }
  });

  inputFields.forEach(function (inputField) {
    if (inputField.dataset.lang) {
      let translations = JSON.parse(inputField.dataset.lang);
      if (translations.hasOwnProperty(lang)) {
        inputField.placeholder = translations[lang];
      }
    }
  });

  buttons.forEach(function (button) {
    if (button.dataset.lang) {
      let translations = JSON.parse(button.dataset.lang);
      if (translations.hasOwnProperty(lang)) {
        button.textContent = translations[lang];
      }
    }
  });

  let searchInputTranslations = JSON.parse(searchInput.dataset.lang || "{}");
  if (searchInputTranslations.hasOwnProperty(lang)) {
    searchInput.placeholder = searchInputTranslations[lang];
  }

  let forgotPasswordTranslations = JSON.parse(
    forgotPasswordLink.dataset.lang || "{}"
  );
  if (forgotPasswordTranslations.hasOwnProperty(lang)) {
    forgotPasswordLink.textContent = forgotPasswordTranslations[lang];
  }

  let showPasswordTranslations = JSON.parse(showPassword.dataset.lang || "{}");
  if (showPasswordTranslations.hasOwnProperty(lang)) {
    showPassword.textContent = showPasswordTranslations[lang];
  }
}

document.addEventListener("DOMContentLoaded", function () {
  let selectedLanguage = localStorage.getItem("selectedLanguage");

  if (selectedLanguage) {
    translateContent(selectedLanguage);
  }
});

function openAddedShortcutModal() {
  let buttons = document.querySelectorAll(".modal-content-shortcuts button");

  if (buttons.length >= 11) {
    alert("You have reached the maximum number of buttons (10).");
    return;
  }
  document.getElementById("added-shortcuts-page-modal").style.display = "block";
  setTimeout(function () {
    document
      .getElementById("added-shortcuts-page-modal")
      .querySelector(".modal-content-added-shortcuts-page")
      .classList.add("show");
  }, 10);
}

let userEmail = "";

// Registration
async function register() {
  const username = document.getElementById("username").value;
  userEmail = document.getElementById("email").value;
  const password = document.getElementById("pass").value.trim();
  const repeatPassword = document.getElementById("repeat-pass").value;

  // Check if all fields are filled
  if (!username.trim()) {
    document.getElementById("username-error").innerText = "Please fill this field";
    return;
  } 
  // else {
  //   document.getElementById("username-error").innerText = "";
  // }

  if (!userEmail.trim()) {
    document.getElementById("email-error").innerText = "Please fill this field";
    return;
  } 
  // else {
  //   document.getElementById("email-error").innerText = ""; 
  // }

  if (!password.trim()) {
    document.getElementById("pass-error").innerText = "Please fill this field";
    return;
  } 
  // else {
  //   document.getElementById("pass-error").innerText = "";
  // }

  if (!repeatPassword.trim()) {
    document.getElementById("repeat-pass-error").innerText = "Please fill this field";
    return;
  } 
  // else {
  //   document.getElementById("repeat-pass-error").innerText = "";
  // }

  // Validate password matching
  if (password !== repeatPassword) {
    document.getElementById("pass-error").innerText = "Passwords do not match";
    return;
  } 
  // else {
  //   document.getElementById("pass-error").innerText = "";
  // }

  // Validate email format
  if (!isValidEmail(userEmail)) {
    document.getElementById("email-error").innerText = "Invalid email format";
    return;
  } 
  else {
    document.getElementById("email-error").innerText = ""; 
  }

  try {
    const response = await fetch("http://localhost:9999/api/register", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ username, email: userEmail, password }),
    });

    const data = await response.json();
    if (data.status === "ok") {
      document.getElementById("user-name").innerText = username;
      document.getElementById("user-email").innerText = userEmail;

      openAccountModal();
    } else {
      alert("Registration failed: " + data.error);
    }
  } catch (error) {
    console.error("Error registering:", error);
    alert("Registration failed");
  }
}

// Login
async function login() {
  userEmail = document.getElementById("login-username").value;
  const login_pass = document.getElementById("login-pass").value;

  // Check if all fields are filled
  if (!userEmail.trim()) {
    document.getElementById("login-username-error").innerText = "Please fill this field";
    return;
  } else {
    document.getElementById("login-username-error").innerText = "";
  }

  if (!login_pass.trim()) {
    document.getElementById("login-pass-error").innerText = "Please fill this field";
    return;
  } else {
    document.getElementById("login-pass-error").innerText = "";
  }

  // Validate email format
  if (!isValidEmail(userEmail)) {
    document.getElementById("login-username-error").innerText = "Invalid email format";
    return;
  } else {
    document.getElementById("login-username-error").innerText = "";
  }

  try {
    // Send POST request to login endpoint
    const response = await fetch("http://localhost:9999/api/login", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ login_username:userEmail, login_pass }),
    });

    // Parse response
    const data = await response.json();

    // Check login status
    if (response.ok) {
      document.getElementById("user-name").innerText = "";
      document.getElementById("user-email").innerText = userEmail;
      openAccountModal();
    } else {
      // Login failed
      alert("Login failed: " + data.error);
    }
  } 
  catch (error) {
    // Handle network errors
    console.error("Error logging in:", error);
    alert("Login failed");
  }
}

// Helper function to validate email format
function isValidEmail(email) {
  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  return emailRegex.test(email);
}

var flag = 1;
function showHide() {
  const show = document.getElementsByClassName("show");
  const hide = document.getElementsByClassName("hide");
  if (flag == 1) {
    flag = 0;
    for (let i = 0; i < show.length; i++) {
      show[i].style.display = "block";
      hide[i].style.display = "none";
    }
  } else {
    flag = 1;
    for (let i = 0; i < show.length; i++) {
      show[i].style.display = "none";
      hide[i].style.display = "block";
    }
  }
  const repeatPassInput = document.getElementById("repeat-pass");
  const passInput = document.getElementById("pass");
  const login_pass = document.getElementById("login-pass");

  // Toggle input type between "password" and "text"
  repeatPassInput.type =
    repeatPassInput.type === "password" ? "text" : "password";
  passInput.type = passInput.type === "password" ? "text" : "password";
  login_pass.type = login_pass.type === "password" ? "text" : "password";
}

async function accountOrLogin() {
  try {
    const response = await fetch('http://localhost:9999/api/accountOrLogin', {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json'
      },
    });

    if (response.status === 200) {
      const data = await response.json();
      setGravatarImage(data.email);
      document.getElementById('user-name').innerText = data.username || '';
      document.getElementById('user-email').innerText = data.email || '';
      openAccountModal();
    } else if (response.status === 404) {
      openLoginModal();
    } else {
      console.error('Failed to fetch user data');
    }
  } catch (error) {
    console.error('Error:', error);
    alert('Failed to fetch user data');
  }
}

async function setGravatarImage(email) {
  try {
    var button = document.querySelector('.acc-container .round-acc-btn');
    var emailHash = getEmailHash(email);
    var imageUrl = `https://www.gravatar.com/avatar/${emailHash}?d=identicon`;
    button.style.backgroundImage = `url(${imageUrl})`;
    button.style.backgroundSize = "cover";
    button.style.backgroundPosition = "center";
  } catch (error) {
    // console.error('Error setting Gravatar image:', error);
    button.style.backgroundImage = "url('default-image.png')";
    button.style.backgroundSize = "cover";
    button.style.backgroundPosition = "center";
  }
}

function getEmailHash(email) {
  // Calculate MD5 hash of email
  var hash = CryptoJS.MD5(email.trim().toLowerCase());
  return hash.toString(CryptoJS.enc.Hex);
}

function addImageToButton() {
  var button = document.getElementById('button');
  button.innerHTML = '<img src="/search_engine/default-profile-account-unknown-icon-black-silhouette-free-vector.jpg" alt="Image">';
}

function openFrogotPassword() {
  window.location.href = "forgot-pass.html";
}

function closeFrogotPassword() {
  document
    .getElementById("forgot-password-page-modal")
    .querySelector(".modal-content-forgot-password-page")
    .classList.remove("show");
  document.getElementById("forgot-password-page-modal").style.display =
    "none";
}


function openChangePassword() {
  window.location.href = "change-pass.html";
}

function closeChangePassword() {
  document
    .getElementById("change-password-page-modal")
    .querySelector(".modal-content-change-password-page")
    .classList.remove("show");
  document.getElementById("change-password-page-modal").style.display =
    "none";
}


// Prevent the space key from acting as a button
// document.getElementById("back-icon").addEventListener("keydown", function (event) {
//   if (event.key === " " || event.key === "Enter") {
//     event.preventDefault();
//   }
// });

// Prevent the space key from acting as a button
document.getElementById("button-container").addEventListener("keydown", function (event) {
  if (event.key === " " || event.key === "Enter") {
    event.preventDefault();
  }
});

// Prevent the space key from acting as a button
// document.getElementById("language").addEventListener("keydown", function (event) {
//   if (event.key === " " || event.key === "Enter") {
//     event.preventDefault();
//   }
// });

function AddImage() {
  window.location.href = "change-account-picture.html";
}

function small_larg() {
  let userInput = document.getElementById("user-input").value;
  if (userInput.length >= 25) {
    let userEmail = document.getElementById("info");
    userEmail.style.fontSize = "1px";
    userEmail.style.justifyContent = "left";
  }

}

function validateInput() {
  var inputField = document.getElementById("link-input");

  if (!inputField.value.trim()) {
    document.getElementById("forget-error-message").innerText = "Please fill this field";
    return;
  } else {
    document.getElementById("forget-error-message").innerText = "";
  }

  openChangePassword();
}

function SaveChanges() {
  var emailCodeInput = document.getElementById("email-code-input");
  var newPasswordInput = document.getElementById("new-password-input");
  var confirmPasswordInput = document.getElementById("confirm-password-input");

  var emailCodeErrorMessage = document.getElementById("email-code-error-message");
  var newPasswordErrorMessage = document.getElementById("new-password-error-message");
  var confirmPasswordErrorMessage = document.getElementById("confirm-password-error-message");

  // Reset error messages
  emailCodeErrorMessage.innerText = "";
  newPasswordErrorMessage.innerText = "";
  confirmPasswordErrorMessage.innerText = "";

  var isValid = true;

  // Validate email code
  if (!emailCodeInput.value.trim()) {
    emailCodeErrorMessage.innerText = "Please enter email code";
    isValid = false;
  }

  // Validate new password
  if (!newPasswordInput.value.trim()) {
    newPasswordErrorMessage.innerText = "Please enter new password";
    isValid = false;
  }

  // Validate confirm password
  if (!confirmPasswordInput.value.trim()) {
    confirmPasswordErrorMessage.innerText = "Please confirm new password";
    isValid = false;
  }
}

// forgot
function SoignOutAccount() {
  openLoginModal();
}