// // g++ -std=c++20 *.cpp -o search_engine -lcurl -lgumbo

// #include <iostream>
// #include <filesystem>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <unordered_map>
// #include <unordered_set>
// #include <algorithm>
// #include <iterator>
// #include <chrono>
// #include <mutex>

// #include "web_crawler.h"
// #include "inverted_index.h"
// #include "search_engine.h"

// int main() {

//     std::vector<std::string> seed_urls = {
//         // "https://www.armstat.am",
//         // "https://www.aua.am",
//         // "https://www.armenianow.com",
//         // "https://www.armeniabusiness.am",
//         // "https://www.armenianheritage.org",
//         // "https://www.moh.am",
//         // "https://www.crrc.am",
//         // "https://www.data.gov.am",
//         // "https://www.meteo.am",
//         // "https://www.armstat.am/en/population",
//         "https://www.picsartacademy.am"
//     };

//     for(int i = 0; i < seed_urls.size(); ++i) {
//         web_crawler crawler(seed_urls[i], i + 1);
//         crawler.start_crawling();
//     }

//     std::string searching_key = obtaining_the_desired_word();

//     const std::string directory_path = std::filesystem::current_path();
//     const std::string file_extension = ".txt";

//     std::vector<std::string> filenames = get_filenames(directory_path, file_extension);

//     // process files
//     std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = build_inverted_index_with_string_docIDs(filenames);
//     display_term_document_matrix_with_terms(inverted_index, filenames);

//     std::unordered_set<std::string> file_id = inverted_index[searching_key]; 

//     std::cout << ".............................." << std::endl;
//     std::cout << file_id.size() << std::endl;
//     for (auto user_id : file_id) {
//         if (crawler.user_links.count(user_id) > 0) {
//             std::cout << user_id << ": " << crawler.user_links[user_id] << std::endl;
//         } else {
//             std::cout << "No link found for user_id: " << user_id << std::endl;
//         }
//     }

//     delete_files(directory_path, file_extension);

//     return 0;
// }

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <mutex>

#include "web_crawler.h"
#include "inverted_index.h"
#include "search_engine.h"

int main() {
    std::vector<std::string> seed_urls = {
        // "https://www.armstat.am",
        // "https://www.aua.am",
        // "https://www.armenianow.com",
        // "https://www.armeniabusiness.am",
        // "https://www.armenianheritage.org",
        // "https://www.moh.am",
        // "https://www.crrc.am",
        // "https://www.data.gov.am",
        // "https://www.meteo.am",
        // "https://www.armstat.am/en/population",
        "https://www.picsartacademy.am"
    };

    std::unordered_map<int, std::set<std::string>> crawlers;

    for (int i = 0; i < seed_urls.size(); ++i) {
        web_crawler crawler(seed_urls[i], i + 1);
        crawler.start_crawling();
        crawlers[crawler.get_id()] = crawler.get_visited_users();
    }

    std::string searching_key = obtaining_the_desired_word();

    const std::string directory_path = std::filesystem::current_path();
    const std::string file_extension = ".txt";

    std::vector<std::string> filenames = get_filenames(directory_path, file_extension);

    // process files
    std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = build_inverted_index_with_string_docIDs(filenames);
    display_term_document_matrix_with_terms(inverted_index, filenames);

    std::unordered_set<std::string> file_id = inverted_index[searching_key];

    for(auto crawler : crawlers) {
        std::cout << crawler.first << " : ";
        for(auto user : crawler.second) {
            std::cout << "\033]8;;" << user << "\a" << user << "\033]8;;\a" << std::endl;
        }
        std::cout << std::endl;
    }

    delete_files(directory_path, file_extension);

    return 0;
}
