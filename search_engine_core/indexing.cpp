#include "indexing.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <iostream>

using namespace mongocxx;

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::kvp;

// Helper function to convert bsoncxx::types::b_utf8 to std::string
std::string bsonValueToString(const bsoncxx::types::b_string &utf8_value)
{
    // return std::string(utf8_value.value);
    return utf8_value.value.to_string();
}

std::string removeFirstCharacter(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    return str.substr(1);
}

std::unordered_map<std::string, std::vector<std::string>> readFromDb(std::string lang) {
    std::unordered_map<std::string, std::vector<std::string>> url_contents;

    try {
        // Connect zto MongoDB
        mongocxx::client conn{mongocxx::uri{}};

        // Access the database and collection
        mongocxx::database db = conn["search-engine"];
        mongocxx::collection coll = db["contents"];

        // Query the collection
        mongocxx::cursor cursor = coll.find({});

        // Check if cursor is empty
        auto begin = cursor.begin();
        auto end = cursor.end();
        if (begin == end) {
            std::cout << "No documents found in the cursor." << std::endl;
            return url_contents;
        }

        for (auto&& doc : cursor) {
            std::string language = doc["language"].get_string().value.to_string();
            if(language == lang) {
                std::string url = doc["url"].get_string().value.to_string();
                bsoncxx::array::view content_view = doc["content"].get_array().value;

                std::vector<std::string> content_vector;
                for (auto&& elem : content_view) {
                    content_vector.push_back(elem.get_string().value.to_string());
                }

                url_contents[url] = content_vector;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return url_contents;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = c + ('a' - 'A');
        }
    }
    
    return result;
}

// std::map<const std::string, std::vector<std::map<std::string, int>>> invertedIndex(
//     const std::unordered_map<std::string, std::unordered_set<std::string>>& urlWordLists) {

//     std::map<const std::string, std::vector<std::map<std::string, int>>> index;

//     for (const auto& pair : urlWordLists) {
//         const std::string& url = pair.first;
//         const std::unordered_set<std::string>& words = pair.second;

//         for (const std::string& word : words) {
//             auto it = index.find(word);
//             if (it == index.end()) {
//                 std::vector<std::map<std::string, int>> urlFlags;
//                 urlFlags.push_back({{url, 1}}); 
//                 index[word] = urlFlags;
//             } else {
//                 bool found = false;
//                 for (auto& urlFlag : it->second) {
//                     if (urlFlag.find(url) != urlFlag.end()) {
//                         urlFlag[url] = 1; 
//                         found = true;
//                         break;
//                     }
//                 }
//                 if (!found) {
//                     it->second.push_back({{url, 1}}); 
//                 }
//             }
//         }
//     }

//     for (const auto& pair : index) {
//         const std::string& word = pair.first;
//         for (const auto& urlWordListPair : urlWordLists) {
//             const std::string& url = urlWordListPair.first;
//             const std::unordered_set<std::string>& words = urlWordListPair.second;
//             if (words.find(word) == words.end()) {
//                 index[word].push_back({{url, 0}});
//             }
//         }
//     }

//     return index;
// }

std::mutex mtx;

void process_content(const std::string& url, const std::vector<std::string>& content,
                     std::map<const std::string, std::vector<std::map<std::string, int>>>& index) {
    std::lock_guard<std::mutex> lock(mtx);

    for (const std::string& word : content) {
        auto it = index.find(word);
        if (it == index.end()) {
            std::vector<std::map<std::string, int>> url_flags;
            url_flags.push_back({{url, 1}});
            index[word] = url_flags;
        } else {
            bool found = false;
            for (auto& url_flag : it->second) {
                if (url_flag.find(url) != url_flag.end()) {
                    url_flag[url] = 1;
                    found = true;
                    break;
                }
            }
            if (!found) {
                it->second.push_back({{url, 1}});
            }
        }
    }
}

std::map<const std::string, std::vector<std::map<std::string, int>>> invertedIndex(
    const std::unordered_map<std::string, std::vector<std::string>>& url_word_lists) {

    std::map<const std::string, std::vector<std::map<std::string, int>>> index;
    std::vector<std::thread> threads;

    for (const auto& url_content_pair : url_word_lists) {
        const std::string& url = url_content_pair.first;
        const std::vector<std::string>& content = url_content_pair.second;

        threads.emplace_back(process_content, url, content, std::ref(index));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (const auto& pair : index) {
        const std::string& word = pair.first;
        for (const auto& url_word_list_pair : url_word_lists) {
            const std::string& url = url_word_list_pair.first;
            const std::vector<std::string>& words = url_word_list_pair.second;
            if (std::find(words.begin(), words.end(), word) == words.end()) {
                index[word].push_back({{url, 0}});
            }
        }
    }

    return index;
}

void writeToMongoDB( const std::map<const std::string, std::vector<std::map<std::string, int>>>& inverted_index, const std::string& lang) {
    try {
        mongocxx::client conn{mongocxx::uri{}};
        
        std::string db_name = "search-engine";
        std::string collection_name = "inverted_index_" + lang;
        auto db = conn[db_name];
        auto collection = db[collection_name];

        collection.delete_many({});

        // int i = 0;
        for (const auto& term_entry : inverted_index) {
            // Construct main document for the term entry
            bsoncxx::builder::basic::document term_doc{};

            std::string word = term_entry.first;

            term_doc.append(kvp("word", word));

            // Construct array of documents (sub-documents for each term occurrence)
            bsoncxx::builder::basic::array term_occurrences{};
            for (const auto& document : term_entry.second) {
                // Construct sub-document for a single term occurrence
                bsoncxx::builder::basic::document doc_entry{};
                for (const auto& entry : document) {
                    doc_entry.append(kvp(entry.first, entry.second));
                }
                term_occurrences.append(doc_entry);
            }

            // Append the array of term occurrences to the main term document
            term_doc.append(kvp("urls", term_occurrences));

            // Insert the term document into MongoDB
            collection.insert_one(term_doc.view());
        }

        std::cout << "Inverted index data successfully written to MongoDB." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to MongoDB: " << e.what() << std::endl;
    }
}

void print_urls_conntent(const std::unordered_map<std::string, std::vector<std::string>>& urls_content) {
    for (const auto &pair : urls_content)
    {
        const std::string &url = pair.first;
        const std::vector<std::string> &content = pair.second;

        std::cout << "URL: " << url << std::endl;
        std::cout << "Content: " << std::endl;
        for(int i = 0; i < content.size(); ++i) {
            std::cout << content[i] << std::endl;
            std::cout << std::endl;
        }
    }
}

void print_inverted_index(const std::map<const std::string, std::vector<std::map<std::string, int>>>& inverted_index) {
    // Print table header
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "| Term         | Document ID | Frequency |" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    int i = 0;
    for (const auto &term_entry : inverted_index)
    {
        const std::string &word = term_entry.first;
        const std::vector<std::map<std::string, int>> &urls = term_entry.second;
        std::cout << ++i << ": " << word << std::endl;

        for (const auto &document : urls)
        {
            auto it = document.begin();
            const std::string &url = it->first;
            int flag = it->second;

            std::cout << flag << std::endl;
        }
    }

    // Print table footer
    std::cout << "-------------------------------------------" << std::endl;
}

