#ifndef Inverted_Index
#define Inverted_Index

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <chrono>

void print_header(const std::vector<std::string>& filenames);
void print_row(const std::string& term, const std::vector<std::string>& documents, const std::unordered_set<std::string>& doc_IDs, std::vector<std::vector<int>>& matrix, int term_index);
void transform_to_lower(std::string& term);
std::unordered_map<std::string, std::unordered_set<std::string>> build_inverted_index_with_string_docIDs(const std::vector<std::string>& filenames);
void display_inverted_index_with_string_docIDs(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index);
void display_term_document_matrix_with_terms(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index, const std::vector<std::string>& filenames);
std::vector<std::string> intersect_terms(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index, std::vector<std::string> terms);

#endif