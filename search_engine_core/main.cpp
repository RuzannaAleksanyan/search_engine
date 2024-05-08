#include "indexing.hpp"
#include <vector>

int main()
{
    // Initialize MongoDB C++ driver
    mongocxx::instance instance{};

    try
    {
        std::vector<std::string> languages = {"arm", "en", "ru"};
        
        for(int i = 0; i < languages.size(); ++i) {
            std::unordered_map<std::string, std::vector<std::string>> urls_content = readFromDb(languages[i]);
            // print_urls_conntent(urls_content);   // test

            // std::unordered_map<std::string, std::unordered_set<std::string>> urls_words_list = separateWordsInTexts(urls_content);
            // print_urls_words_list(urls_words_list);  // test

            const std::map<const std::string, std::vector<std::map<std::string, int>>> inverted_index = invertedIndex(urls_content);
            // print_inverted_index(inverted_index);    // test

            writeToMongoDB(inverted_index, languages[i]);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
