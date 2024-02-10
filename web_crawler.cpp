#include "web_crawler.h"
#include <fstream>
#include <iostream>
#include <regex>

web_crawler::web_crawler(const std::string& api_endpoint, int num)
    : api_endpoint(api_endpoint), ID(num) {
}

web_crawler::~web_crawler() {
    if (thread.joinable()) {
        thread.join();
    }
}

void web_crawler::start_crawling() {
    int count = 0;
    crawl(api_endpoint, 0, count);
}

size_t web_crawler::write_callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(reinterpret_cast<char*>(contents), totalSize);
    return totalSize;
}

void web_crawler::write_content_to_file(const std::string& filename, const std::string& content) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << content;
        outfile.close();
        // std::cout << "Content written to file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
    }
}

std::set<std::string> web_crawler::extract_links(const std::string& htmlContent) {
    std::set<std::string> links;
    std::regex linkRegex("<a\\s+[^>]*\\s*href=[\"']([^\"']+)[\"'][^>]*>");

    std::sregex_iterator linkBegin(htmlContent.begin(), htmlContent.end(), linkRegex);
    std::sregex_iterator linkEnd;

    for (std::sregex_iterator it = linkBegin; it != linkEnd; ++it) {
        std::smatch match = *it;

        if (match.size() >= 2) {
            std::string link = match[1].str();
            links.insert(link);
        }
    }

    return links;
}

void web_crawler::collect_user_data(const std::string& api_url) {
    // Check if apiUrl has the same domain as apiEndpoint
    if (!is_same_domain(api_url, api_endpoint)) {
        return;
    }

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string response_body;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            std::cout << "\033]8;;" << api_url << "\a" << api_url << "\033]8;;\a" << std::endl;

            visited_users.insert(api_url);

            std::set<std::string> links = extract_links(response_body);

            for (const std::string& link : links) {
                if (visited_users.count(link) == 0) {
                    collect_user_data(link);
                }
            }

            inker_content = response_body;
        }

        curl_easy_cleanup(curl);
    }
}

bool web_crawler::is_same_domain(const std::string& url1, const std::string& url2) {
    std::regex domainRegex(R"((https?://([^/]+)))");
    std::smatch match1, match2;

    if (std::regex_search(url1, match1, domainRegex) && std::regex_search(url2, match2, domainRegex)) {
        return match1[2] == match2[2];
    }

    return false;
}

void web_crawler::crawl(const std::string& api_url, int depth, int& crawl_count) {
    if (crawl_count >= 15 || depth > MAX_DEPTH || visited_users.count(api_url) > 0) {
        return;
    }

    collect_user_data(api_url);

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string response_body;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            std::set<std::string> links = extract_links(response_body);
            visited_users.insert(api_url);

            GumboOutput* output = gumbo_parse(response_body.c_str());
            std::string text_content = get_text_content(output->root);

            std::string filename = "page_" + std::to_string(ID) + "_" + std::to_string(visited_users.size()) + ".txt";
            write_content_to_file(filename, text_content);

            gumbo_destroy_output(&kGumboDefaultOptions, output);

            for (const std::string& link : links) {
                if (crawl_count < 30) {
                    crawl(link, depth + 1, ++crawl_count);
                } else {
                    break;
                }
            }
        }
        curl_easy_cleanup(curl);
    }
}



std::string web_crawler::get_text_content(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return node->v.text.text;
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {
        std::string text_content;
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            std::string child_text_content = get_text_content(static_cast<GumboNode*>(children->data[i]));
            text_content += child_text_content;
        }
        return text_content;
    } else {
        return "";
    }
}