#include "inverted_index.h"

// void print_header(const std::vector<std::string>& filenames) {
//     std::cout << std::setw(30) << "";
//     for (const std::string& document : filenames) {
//         std::cout << std::setw(16) << document;
//     }
//     std::cout << "\n-----------------------------------------------------------------------------------------------------\n";
// }

// void print_row(const std::string& term, const std::vector<std::string>& documents, const std::unordered_set<std::string>& doc_IDs, std::vector<std::vector<int>>& matrix, int term_index) {
//     std::cout << std::setw(30) << term;
//     for (int doc_index = 0; doc_index < documents.size(); doc_index++) {
//         if (doc_IDs.count(documents[doc_index]) > 0) {
//             matrix[term_index][doc_index] = 1;
//             std::cout << std::setw(16) << "1";
//         } else {
//             std::cout << std::setw(16) << "0";
//         }
//     }
//     std::cout << std::endl;
// }

void print_header(const std::vector<std::string>& filenames) {
    std::cout << std::setw(40) << " ";
    for (const std::string& document : filenames) {
        std::cout << std::setw(16) << std::left << document;
    }
    std::cout << "\n-----------------------------------------------------------------------------------------------------\n";
}

void print_row(const std::string& term, const std::vector<std::string>& documents, const std::unordered_set<std::string>& doc_IDs, std::vector<std::vector<int>>& matrix, int term_index) {
    std::cout << std::setw(35) << std::left << term;
    for (int doc_index = 0; doc_index < documents.size(); doc_index++) {
        if (doc_IDs.count(documents[doc_index]) > 0) {
            matrix[term_index][doc_index] = 1;
            std::cout << std::setw(16) << "1";
        } else {
            std::cout << std::setw(16) << "0";
        }
    }
    std::cout << std::endl;
}


void transform_to_lower(std::string& term) {
    std::transform(term.begin(), term.end(), term.begin(), ::tolower);
}

std::unordered_map<std::string, std::unordered_set<std::string>> build_inverted_index_with_string_docIDs(const std::vector<std::string>& filenames) {
    std::unordered_map<std::string, std::unordered_set<std::string>> inverted_index;

    for (int docID = 0; docID < filenames.size(); docID++) {
        std::ifstream file(filenames[docID]);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::vector<std::string> terms{ std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>() };

                std::for_each(terms.begin(), terms.end(), transform_to_lower);
                std::sort(terms.begin(), terms.end());

                for (const std::string& term : terms) {
                    inverted_index[term].insert(std::to_string(docID + 1));
                }
            }
            file.close();
        } else {
            std::cerr << "Error opening file: " << filenames[docID] << std::endl;
        }
    }
    return inverted_index;
}

void display_inverted_index_with_string_docIDs(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index) {
    std::cout << "Inverted Index Representation:\n";
    for (const auto& entry : inverted_index) {
        std::cout << entry.first << ": ";
        for (const auto& docID : entry.second) {
            std::cout << docID << " ";
        }
        std::cout << std::endl;
    }
}

void display_term_document_matrix_with_terms(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index, const std::vector<std::string>& filenames) {
    std::unordered_set<std::string> unique_terms;
    for (const auto& entry : inverted_index) {
        unique_terms.insert(entry.first);
    }

    std::vector<std::string> documents;
    for (const auto& docIDs : inverted_index) {
        for (const auto& docID : docIDs.second) {
            documents.push_back(docID);
        }
    }
    std::sort(documents.begin(), documents.end());
    documents.erase(std::unique(documents.begin(), documents.end()), documents.end());

    std::vector<std::vector<int>> matrix(unique_terms.size(), std::vector<int>(documents.size(), 0));

    print_header(filenames);

    std::vector<std::string> sorted_terms(unique_terms.begin(), unique_terms.end());
    std::sort(sorted_terms.begin(), sorted_terms.end());

    for (int term_index = 0; term_index < unique_terms.size(); term_index++) {
        std::string term = sorted_terms[term_index];
        const auto& docIDs = inverted_index.find(term)->second;

        print_row(term, documents, docIDs, matrix, term_index);
    }
}

std::vector<std::string> intersect_terms(const std::unordered_map<std::string, std::unordered_set<std::string>>& inverted_index, std::vector<std::string> terms) {
    std::sort(terms.begin(), terms.end(), [&inverted_index](const std::string& term1, const std::string& term2) {
        return inverted_index.find(term1)->second.size() < inverted_index.find(term2)->second.size();
    });

    std::vector<std::string> result(inverted_index.find(terms[0])->second.begin(), inverted_index.find(terms[0])->second.end());

    for (int i = 1; i < terms.size() && !result.empty(); i++) {
        const auto& current_postings = inverted_index.find(terms[i])->second;
        std::vector<std::string> intersection;
        std::set_intersection(result.begin(), result.end(), current_postings.begin(), current_postings.end(), std::back_inserter(intersection));
        result = intersection;
    }
    return result;
}