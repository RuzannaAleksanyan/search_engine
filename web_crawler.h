#ifndef WEB_CRAWLER_H
#define WEB_CRAWLER_H

#include <set>
#include <string>
#include <thread>
#include <curl/curl.h>

#include "gumbo.h"

class web_crawler {
private:
    static const int MAX_DEPTH = 3;
    std::string api_endpoint;
    int ID;
    std::thread thread;
    std::set<std::string> visited_users;
    std::string inker_content;

public:
    web_crawler(const std::string& api_endpoint, int num);
    ~web_crawler();

    void start_crawling();

private:
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output);
    void write_content_to_file(const std::string& filename, const std::string& content);
    std::set<std::string> extract_links(const std::string& htmlContent);
    void collect_user_data(const std::string& api_url);
    void crawl(const std::string& api_url, int depth, int& crawl_count);
    bool is_same_domain(const std::string& url1, const std::string& url2);

    std::string get_text_content(GumboNode* node);
};

#endif // WEB_CRAWLER_H
