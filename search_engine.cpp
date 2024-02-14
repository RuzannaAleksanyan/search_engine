#include "search_engine.h"

std::mutex str_mutex;

std::string obtaining_the_desired_word() {
    httplib::Server svr;

    std::string str = "";

    svr.Get("/extractWords", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Origin, Content-Type, Accept");

        std::string query_string = req.get_param_value("query");

        std::istringstream iss(query_string);

        std::vector<std::string> words;

        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }

        std::lock_guard<std::mutex> lock(str_mutex);
        for (size_t i = 0; i < words.size(); ++i) {
            str += words[i];
            if (i < words.size() - 1) {
                str += ", ";
            }
        }

        res.set_content(str.c_str(), "application/json");  // Use str.c_str() to get a C-style string
    });

    std::thread serverThread([&]() {
        svr.listen("localhost", 8080);
    });

    std::this_thread::sleep_for(std::chrono::seconds(20));

    // Stop the server after 20 seconds
    svr.stop();

    // Join the server thread
    serverThread.join();

    // Lock the mutex before accessing str in the main thread
    std::lock_guard<std::mutex> lock(str_mutex);

    return str;
}

void delete_files(const std::string& directory_path, const std::string& file_extension) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            // Check if the file has the desired extension
            if (entry.path().extension() == file_extension) {
                // Delete the file
                std::filesystem::remove(entry.path());
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << std::endl;
    }
}

std::vector<std::string> get_filenames(const std::string& directory_path, const std::string& file_extension) {
    std::vector<std::string> filenames;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.path().extension() == file_extension) {
                filenames.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << std::endl;
    }

    return filenames;
}

bool is_valid_word(const std::string& word1, const std::string& word2) {
    if (word1.empty()) {
        return false;
    }

    char lastChar = word2.empty() ? '\0' : word2.back();

    return std::isupper(word1.front()) && (lastChar != '.' && lastChar != ';' && lastChar != '?' && lastChar != '!' && lastChar != ',');
}

void process_file(const std::vector<std::string>& filenames) {
    for (const std::string& filename : filenames) {
        std::ifstream input_file(filename);

        if (!input_file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            continue;
        }

        std::vector<std::string> words;
        std::string word;

        // Read words from file
        while (input_file >> word) {
            words.push_back(word);
        }

        input_file.close();

        // Filter and write back valid words
        std::ofstream output_file(filename);

        for (int i = 0; i < words.size(); ++i) {
            if (i > 0 && is_valid_word(words[i], words[i - 1])) {
                int size = words[i].size();
                if (size > 0 && (words[i][size - 1] == '.' || words[i][size - 1] == ',' || words[i][size - 1] == '?' || words[i][size - 1] == ':' || words[i][size - 1] == '!' || words[i][size - 1] == '`')) {
                    words[i].pop_back();
                }

                words[i][0] = std::tolower(words[i][0]);

                output_file << words[i] << " ";
            }
        }

        output_file.close();
    }
}



