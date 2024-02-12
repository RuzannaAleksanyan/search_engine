#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE

#include <string>
#include <thread>
#include "httplib.h"

std::string obtaining_the_desired_word();
void delete_files(const std::string& directory_path, const std::string& file_extension);
std::vector<std::string> get_filenames(const std::string& directory_path, const std::string& file_extension);

#endif  // SEARCH_ENGINE 