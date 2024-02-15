// g++ -std=c++20 *.cpp -o search_engine -lcurl -lgumbo

#include "web_crawler.h"
#include "inverted_index.h"
#include "search_engine.h"

#include <cstdlib>  // For system function



int main() {
    std::unordered_map<int, std::set<std::string>> crawlers;

    std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index = perform_web_crawling_and_indexing(crawlers);

    std::string searching_key = obtaining_the_desired_word();

    // std::unordered_set<std::string> file_id = inverted_index[searching_key];

    std::cout << "......................." << std::endl;
    std::cout << searching_key << std::endl;

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
