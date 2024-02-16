#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE

#include <string>
#include <thread>
#include "httplib.h"
#include "web_crawler.h"
#include "inverted_index.h"

extern std::mutex str_mutex;
extern std::string str;

int start_search_engine(const std::string& searching_key);

std::string obtaining_the_desired_word();
void delete_files(const std::string& directory_path, const std::string& file_extension);
std::vector<std::string> get_filenames(const std::string& directory_path, const std::string& file_extension);

bool is_valid_word(const std::string& word1, const std::string& word2);
void process_file(const std::vector<std::string>& filenames);

std::unordered_map<std::string, std::unordered_set<std::string>> perform_web_crawling_and_indexing(std::unordered_map<int, std::set<std::string>>& crawlers);

#endif  // SEARCH_ENGINE 