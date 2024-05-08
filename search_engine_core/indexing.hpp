#ifndef INDEXING
#define INDEXING

#include <iostream>
#include <map>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <sstream>
#include <unordered_set>
#include <cctype>

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>



std::string bsonValueToString(const bsoncxx::types::b_utf8 &utf8_value);
std::string removeFirstCharacter(const std::string& str);
std::unordered_map<std::string, std::vector<std::string>> readFromDb(std::string lang);
std::string toLower(const std::string& str);
// std::unordered_map<std::string, std::unordered_set<std::string>> separateWordsInTexts(
//                         const std::unordered_map<std::string, std::string>& urlContents);
std::map<const std::string, std::vector<std::map<std::string, int>>> invertedIndex(
    const std::unordered_map<std::string, std::vector<std::string>>& url_word_lists);
void writeToMongoDB( const std::map<const std::string, std::vector<std::map<std::string, int>>>& inverted_index, const std::string& lang);


// test
void print_urls_conntent(const std::unordered_map<std::string, std::vector<std::string>>& urls_content);
void print_urls_words_list(const std::unordered_map<std::string, std::unordered_set<std::string>>& urls_words_list);
void print_inverted_index(const std::map<const std::string, std::vector<std::map<std::string, int>>>& inverted_index);

#endif  // INDEXING