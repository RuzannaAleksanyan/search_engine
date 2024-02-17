#include "search_engine.h"

int start_search_engine(const std::string& searching_key) {
    std::cout << "% " << std::endl;

    // std::unordered_map<int, std::set<std::string>> crawlers;

    // std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = perform_web_crawling_and_indexing(crawlers);

    // std::string searching_key = obtaining_the_desired_word();

    // std::unordered_set<std::string> file_id = inverted_index[searching_key];

    // std::cout << "......................." << std::endl;
    // std::cout << searching_key << std::endl;

    // // ...
    // std::cout << "......................" << std::endl;
    // for(auto crawler : crawlers) {
    //     for(auto link : crawler.second) {
    //         std::cout << "\033]8;;" << link << "\a" << link << "\033]8;;\a" << std::endl;
    //     }
    // }

    // for(auto crawler : crawlers) {
    //     for(auto id : file_id) {
    //         if(crawler.first == std::stoi(id)) {
    //             for(auto link : crawler.second) {
    //                 std::cout << "\033]8;;" << link << "\a" << link << "\033]8;;\a" << std::endl;
    //             }
    //         }
    //     }
    // }


    return 0;
}

std::mutex str_mutex;
std::string str;

std::string obtaining_the_desired_word() {
    httplib::Server svr;

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

        res.set_content(str.c_str(), "application/json");
    });

    std::thread serverThread([&]() {
        svr.listen("localhost", 8080);
    });

    std::this_thread::sleep_for(std::chrono::seconds(20));

    svr.stop();

    serverThread.join();

    std::lock_guard<std::mutex> lock(str_mutex);

    return str;
}

std::unordered_map<std::string, std::unordered_set<std::string>> perform_web_crawling_and_indexing(std::unordered_map<int, std::set<std::string>>& crawlers) {
    std::vector<std::string> seed_urls = {
        // "https://www.armstat.am",
        // "https://www.aua.am",
        // "https://www.armenianow.com",
        // "https://www.armenianheritage.org",
        // "https://www.moh.am",
        // "https://www.crrc.am",
        "https://www.picsartacademy.am"
    };

    for (int i = 0; i < seed_urls.size(); ++i) {
        web_crawler crawler(seed_urls[i], i + 1);
        crawler.start_crawling();
        crawlers[crawler.get_id()] = crawler.get_visited_users();
    }

    const std::string directory_path = std::filesystem::current_path();
    const std::string file_extension = ".txt";

    std::vector<std::string> filenames = get_filenames(directory_path, file_extension);
    process_file(filenames);

    // process files
    std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = build_inverted_index_with_string_docIDs(filenames);
    
    // display_term_document_matrix_with_terms(inverted_index, filenames);

    delete_files(directory_path, file_extension);

    return inverted_index;
}

void delete_files(const std::string& directory_path, const std::string& file_extension) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.path().extension() == file_extension) {
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

        while (input_file >> word) {
            words.push_back(word);
        }

        input_file.close();

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



