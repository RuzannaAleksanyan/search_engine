// localStorage.clear();

const searchTerms = new URLSearchParams(window.location.search).get("searchTerm");

fetch("http://localhost:9999/api/search", {
  method: "POST",
  headers: {
    "Content-Type": "application/json",
  },
  body: JSON.stringify({ searchTerms }),
})
.then((r) => r.json())
.then((data) => {
  // console.log("hello");
  let res = [];
  if (data?.data) {
    const results = data.data;

    for (const key in results) {
      let array = results[key];
      for (let i = 0; i < array.length; ++i) {
        const element = array[i];

        for (const url in element) {
          const value = element[url];

          if (value === 1) {
            res.push(url);
          }
        }
      }
    }

    if (res.length > 0) {
      console.log('display urls');
      displayURLs(res);
    } else {
      console.log('not found');
      searchResultNotFound();
    }
  } else {
    console.log("Invalid data format received.");
  }
});

const searchInput = document.getElementById("second-page-input");
searchInput.addEventListener("keydown", async function (event) {
  if (event.key === "Enter") {
    window.location.href = `second_page.html?searchTerm=${searchInput.value.trim()}`;
  }
});

function searchResultNotFound() {
  const container = document.getElementById("url-container");
  container.innerHTML = "";

  const messageDiv = document.createElement("div");
  messageDiv.classList.add("search-message");

  const heading = document.createElement("h2");
  heading.textContent =
    "No results containing all your search terms were found.";
  messageDiv.appendChild(heading);

  const paragraph = document.createElement("p");
  paragraph.textContent = "Your search did not match any documents.";
  messageDiv.appendChild(paragraph);

  const suggestionsList = document.createElement("ul");
  const suggestions = [
    "Make sure that all words are spelled correctly.",
    ". Try different keywords.",
    ". Try more general keywords.",
    ". Try fewer keywords.",
  ];

  suggestions.forEach((suggestion) => {
    const listItem = document.createElement("li");
    listItem.textContent = suggestion;
    suggestionsList.appendChild(listItem);
  });

  messageDiv.appendChild(suggestionsList);

  container.appendChild(messageDiv);
}

function displayURLs(urls) {
  if (urls?.length < 1) {
    return;
  }
  const container = document.getElementById("url-container");
  container.innerHTML = "";

  const list = document.createElement("ul");

  urls.forEach((url) => {
    const listItem = document.createElement("li");

    const link = document.createElement("a");
    link.href = url;
    link.textContent = url;

    listItem.appendChild(link);
    list.appendChild(listItem);
  });

  container.appendChild(list);

  localStorage.setItem("displayedURLs", JSON.stringify(urls));
}

// document.addEventListener("DOMContentLoaded", function () {
//   const storedURLs = localStorage.getItem("displayedURLs");
//   if (storedURLs) {
//     const urls = JSON.parse(storedURLs);
//     // displayURLs(urls);
//   }
// });

window.addEventListener("beforeunload", function (event) {
  if (!enterPressed) {
    const displayedURLs = [];
    const listItems = document.querySelectorAll("#url-container ul li a");
    listItems.forEach((item) => {
      displayedURLs.push(item.href);
    });
    localStorage.setItem("displayedURLs", JSON.stringify(displayedURLs));
  }
});

// Opens the login modal on the second page
function openLoginModalSecondPage() {
  closeRegistrationModal();
  closeLanguageModal();
  closeSettingsModal();

  let modal = document.getElementById("login-page-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

// Opens the settings modal on the second page
function openSettingsModalSecondPage() {
  closeLanguageModal();
  closeLoginModal();
  closeRegistrationModal();

  const modal = document.getElementById("settings-modal");
  if (modal.style.display === "block") {
    modal.style.display = "none";
  } else {
    modal.style.display = "block";
  }
}

function openLanguagePageSecondModal() {
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

// document.addEventListener("DOMContentLoaded", function () {

// });

const urlString = window.location.href;
const url = new URL(urlString);
const queryParams = url.searchParams;
const paramValue = queryParams.get("searchTerm");
// console.log(paramValue);
setTimeout(() => {
  const inputDOM = document.querySelector("#second-page-input");
  inputDOM.value = paramValue;
}, 1);
