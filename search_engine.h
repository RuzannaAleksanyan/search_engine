#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE

#include <string>
#include "httplib.h"
#include <thread>

std::string clean_and_organize_URL();
void delete_files(const std::string& directory_path, const std::string& file_extension);
std::vector<std::string> get_filenames(const std::string& directory_path, const std::string& file_extension);

#endif  // SEARCH_ENGINE 