#ifndef _SERVER
#define _SERVER

#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <mutex>
#include <sstream>
#include <atomic>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "web_crawler.h"
#include "inverted_index.h"
#include "search_engine.h"

// #include "httplib.h"
#include <thread>
// #include <vector>



struct client_t {
    sockaddr_in address;
    int sockfd;
    int uid;
    char name[32];
};


void queue_add(client_t *cl);
// void send_message(const char *s, int uid);
void str_trim_lf(char *arr, int length);
void queue_remove(int uid);
std::string get_last_line(const std::string& text);
void* handle_client(void *arg);
int start_server(int argc, char **argv);

#endif  // _SERVER