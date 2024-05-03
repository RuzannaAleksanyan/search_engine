var infoText = document.querySelectorAll("#info p");
infoText.forEach(function (element) {
  element.style.transform = "translateX(0)";
});

// Ensures the translation occurs after DOM content is loaded
document.addEventListener("DOMContentLoaded", function () {
  var infoText = document.querySelectorAll("#info p");
  infoText.forEach(function (element) {
    element.style.transform = "translateX(0)";
  });
});
