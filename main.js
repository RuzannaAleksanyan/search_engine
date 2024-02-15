const button = document.querySelector('.button');

const extractWords = () => {
    const input = document.querySelector('.queryString');
    const queryString = input.value;

    fetch(`http://127.0.0.1:5501/extractWords?query=${queryString}`)
    .then(r => {
        console.log('r: ', r);
        return r.json();
    })
    .then(d => {
        console.log('d: ', d);
    })
    .catch(e => console.log('Error'));

    // const xhr = new XMLHttpRequest();
    // xhr.open("GET", "http://localhost:8080/extractWords?query=" + queryString, true);
    // xhr.onreadystatechange = function () {
    //     if (xhr.readyState == 4) {
    //         console.log("Response received:", xhr.responseText);
    //     }
    // };
    // xhr.send();
}

button.addEventListener("click", () => {
    extractWords();
});

document.addEventListener('keydown', (e) => {
    if (e.key === "Enter") {
        extractWords();
    }
});