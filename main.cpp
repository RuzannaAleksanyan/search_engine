// g++ -std=c++20 *.cpp -o search_engine -lcurl -lgumbo

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

    // std::vector<std::string> seed_urls = {
    //     "https://www.armstat.am",
    //     "https://www.aua.am",
    //     "https://www.armenianow.com",
    //     "https://www.armeniabusiness.am",
    //     "https://www.armenianheritage.org",
    //     "https://www.moh.am",
    //     "https://www.crrc.am",
    //     "https://www.data.gov.am",
    //     "https://www.meteo.am",
    //     "https://www.armstat.am/en/population"
    // };

    std::string start_url = clean_and_organize_URL();

    web_crawler crawler(start_url, 1);
    crawler.start_crawling();

    const std::string directory_path = std::filesystem::current_path();
    const std::string file_extension = ".txt";

    std::vector<std::string> filenames = get_filenames(directory_path, file_extension);

    // process files
    std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = build_inverted_index_with_string_docIDs(filenames);
    display_term_document_matrix_with_terms(inverted_index, filenames);

    delete_files(directory_path, file_extension);

    return 0;
}

