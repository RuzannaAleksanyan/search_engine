// function extractWords() {
//     const queryString = document.getElementById("queryString").value;

//     // Make a POST request to the C++ server
//     fetch("http://localhost:3000/extractWords", {
//         method: "POST",
//         headers: {
//             "Content-Type": "application/json",
//         },
//         body: `query=${encodeURIComponent(queryString)}`,
//     })
//     .then(response => response.json())
//     .then(data => {
//         // Handle the response data
//         console.log("Received data from server:", data);

//         // Update the UI or perform other actions
//         // displayResults(data);
//     })
//     .catch(error => {
//         console.error("Error:", error);
//     });
// }

function extractWords() {
    const queryString = document.getElementById("queryString").value;

    // Make a POST request to the C++ server
    fetch("http://localhost:3000/extractWords", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({ query: queryString }), // Send the query string as JSON
    })
    .then(response => response.json())
    .then(data => {
        // Handle the response data
        console.log("Received data from server:", data);

        // Update the UI or perform other actions
        // displayResults(data);
    })
    .catch(error => {
        console.error("Error:", error);
    });
}


function displayResults(data) {
    // Update the UI to display the results
    const outputList = document.getElementById("outputList");
    outputList.innerHTML = ""; // Clear previous results

    for (const result of data.results) {
        const listItem = document.createElement("li");
        listItem.textContent = result;
        outputList.appendChild(listItem);
    }
}
