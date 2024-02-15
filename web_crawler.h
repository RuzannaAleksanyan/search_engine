#ifndef WEB_CRAWLER_H
#define WEB_CRAWLER_H

#include <iostream>
#include <fstream>
#include <set>
#include <regex>
#include <curl/curl.h>
#include <thread>
#include <condition_variable>

#include "gumbo.h"

class web_crawler {
private:
    // static std::function<void(GumboNode*, std::string&)> extractText;

    static const int MAX_DEPTH = 15;
    std::string api_endpoint;
    int ID;
    std::thread thread;
    mutable std::mutex mutex;
    std::set<std::string> visited_users;
    std::string inker_content;

public:
    web_crawler(const std::string& api_endpoint, int num);
    ~web_crawler();

    void start_crawling();

    const std::set<std::string>& get_visited_users() const;
    int get_id() const;

private:
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output);
    void write_content_to_file(const std::string& filename, const std::string& content);
    std::set<std::string> extract_links(const std::string& htmlContent);
    void collect_user_data(const std::string& api_url);
    void crawl(const std::string& api_url, int depth, int& crawl_count);
    bool is_same_domain(const std::string& url1, const std::string& url2);

    std::string get_text_content(GumboNode* node); 

    bool is_valid_url(const std::string& url) {
        // Regular expression for basic URL validation
        std::regex urlRegex("(https?|ftp)://[^\\s/$.?#].[^\\s]*");

        return std::regex_match(url, urlRegex);
    }

    size_t redirect_callback(void* contents, size_t size, size_t nmemb, std::string* output);

    bool is_facebook_redirect(const std::string& url);

};

#endif // WEB_CRAWLER_H
