// const mongoose = require("mongoose");
const express = require("express");
const path = require("path");
const bodyParser = require("body-parser");
const User = require("./model/user");
const InvertedIndex = require("./model/inverted-index");
const bcrypt = require("bcryptjs");
const jwt = require("jsonwebtoken");
const { connect } = require("mongoose");
const cors = require("cors");
require('dotenv').config()

// jwt
const JWT_SECRET = "sdjkfh8923yhjdksbfma@#*(&@*!#&@bhjb2qiuhesdbhjdsfg839ujkdhfjk";

async function connectToDatabase() {
  try {
    // Start the database connection.
    await connect("mongodb://localhost:27017/search-engine" || "");
    console.log("===== Connected to MongoDB ======");
    console.log(`=================================`);
  } catch (error) {
    console.error("Error connecting to database: ", error);
  }
}

connectToDatabase();

// Create an Express application instance.
const app = express();

app.use(cors());

// Serves static files from the `static` directory.
app.use("/", express.static(path.join(__dirname, "static")));

// Analyze incoming request bodies.
app.use(bodyParser.json());

// Define route processing POST requests to /api/login
app.post("/api/login", async (req, res) => {
  const { login_username, login_pass } = req.body;
  const user = await User.findOne({ email: login_username }).lean();

  if (!user) {
    return res.json({ status: 400, error: "Invalid login_username or password" });
  }

  if (await bcrypt.compare(login_pass, user.password)) {
    // the username, password combination is successful
    const user_data = { id: user._id, login_username: user.login_username, email: user.email };
    const token = jwt.sign(
      user_data,
      JWT_SECRET
    );
    return res.json({ status: 200, data: { token, user_data } });
  }

  return res.json({ status: 400, error: "Invalid login_username or login_pass 2" });
});

app.post("/api/register", async (req, res) => {
  const { username, email, password } = req.body;

  try {
    // Hash the password
    const hashedPassword = await bcrypt.hash(password, Number(process.env.BCRYPTSALT));

    // Create a new user in the database with empty shortcut and history arrays
    const newUser = await User.create({
      username,
      email,
      password: hashedPassword,
      flag: "1",
      image: "/search-engine-frontend/images.png",
      shortcut: [], // Empty array for shortcut
      history: []   // Empty array for history
    });

    console.log("User created successfully:", newUser);
    res.json({ status: "ok" });
  } catch (error) {
    if (error.code === 11000) {
      return res.json({ status: "error", error: "Username or email already in use" });
    }
    console.error("Error creating user:", error);
    res.json({ status: "error", error: "An error occurred" });
  }
});

// Define route processing GET requests to /api/accountOrLogin
app.get("/api/accountOrLogin", async (req, res) => {
  try {
    const user = await User.findOne({ flag: "1" }).lean();

    if (user) {
      return res.json(user);
    } else {
      return res.status(404).json({ status: 404, error: "No user with flag equal to one found" });
    }
  } catch (error) {
    console.error(error);
    return res.status(500).json({ status: 500, error: "Internal server error" });
  }
});

app.post("/api/search", async (req, res) => {
  // vercnel req.body.searchTerm-y harcwum anel db-in hamapatasxan linkery veradarcnel front amen bari hamar
  const searchTerms = req.body.searchTerms;
  console.log(searchTerms);

  
  try {
    const relevantWords = await InvertedIndex.find({ word: { $in: searchTerms } });
    // console.log(relevantWords);

    // Prepare response data containing URLs and their corresponding words
    const responseData = {};

    
    // Iterate through each relevant word document
    relevantWords.forEach(wordDoc => {
      const word = wordDoc.word;
      const urls = wordDoc.urls;


      // If word is not yet a key in responseData, initialize it with an empty array
      if (!responseData[word]) {
        responseData[word] = [];
      }

      // Push each URL associated with the word into the responseData[word] array
      responseData[word].push(...urls);
    });

    // console.log(searchTerms, responseData.s);
    console.log(responseData);
    return res.status(200).json({ status: 200, data: responseData });
  } catch (error) {
    console.error("Error searching inverted index:", error);
    return res.status(500).json({ status: 500, message: "Internal server error" });
  }
});

const PORT = 9999;

const server = app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});

// Run the Express application
server.on('error', (error) => {
  if (error.code === 'EADDRINUSE') {
    console.warn(`Port ${PORT} is already in use. Trying another port...`);
    server.listen(0); // This will choose a random available port
  } else {
    console.error('Server error:', error);
  }
});
