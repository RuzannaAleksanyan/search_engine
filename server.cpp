// g++ -std=c++20 *.cpp -o search_engine -lcurl -lgumbo

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

#define MAX_CLIENTS 100
#define BUFFER_SIZE 2048

std::atomic<unsigned int> client_count{0};
int uid = 10;

struct client_t {
    sockaddr_in address;
    int sockfd;
    int uid;
    char name[32];
};

client_t *clients[MAX_CLIENTS];
std::mutex clients_mutex;

void print_client_addr(sockaddr_in addr) {
    // std::cout << static_cast<int>((addr.sin_addr.s_addr & 0xff)) << '.'
    //           << static_cast<int>((addr.sin_addr.s_addr & 0xff00) >> 8) << '.'
    //           << static_cast<int>((addr.sin_addr.s_addr & 0xff0000) >> 16) << '.'
    //           << static_cast<int>((addr.sin_addr.s_addr & 0xff000000) >> 24);
}

void queue_add(client_t *cl) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
}

void send_message(const char *s, int uid) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            if (clients[i]->uid != uid) {
                if (write(clients[i]->sockfd, s, strlen(s)) < 0) {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }
}

void str_trim_lf(char *arr, int length) {
    for (int i = 0; i < length; i++) {
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void queue_remove(int uid) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            if (clients[i]->uid == uid) {
                clients[i] = nullptr;
                break;
            }
        }
    }
}

void handle_received_message(const char* message, int uid) {
    // std::cout << "Received message from client " << uid << " (" << clients[uid]->name << "): " << message << std::endl;

    std::string response = "{\"result\": \"Sample response\"}";
    // std::cout << response.c_str();
    send_message(response.c_str(), uid);

    std::string received_message(message);
    // std::cout << ".............1" << "2" << std::endl;
    start_search_engine(received_message);
}

void *handle_client(void *arg) {
    char buff_out[BUFFER_SIZE];
    int leave_flag = 0;

    client_count++;
    client_t *client = static_cast<client_t *>(arg);

    // Initialize name directly
    std::ostringstream oss;
    oss << "Client_" << client->uid;  // You can customize the naming convention
    strcpy(client->name, oss.str().c_str());

    // std::cout << client->name << " has joined\n";
    send_message("You have joined the chat\n", client->uid);

    bzero(buff_out, BUFFER_SIZE);

    while (1) {
        if (leave_flag) {
            break;
        }

        int receive = recv(client->sockfd, buff_out, BUFFER_SIZE, 0);
        if (receive > 0) {
            if (strlen(buff_out) > 0) {
                send_message(buff_out, client->uid);

                str_trim_lf(buff_out, strlen(buff_out));
                // std::cout << buff_out << " -> " << client->name << '\n';

                // Process the received message
                handle_received_message(buff_out, client->uid);
                std::cout << std::endl << "1" << buff_out << "2" << std::endl;
            }
        } else if (receive == 0 || strcmp(buff_out, "exit") == 0) {
            std::ostringstream oss;
            oss << client->name << " has left\n";
            std::string leaveMessage = oss.str();
            strcpy(buff_out, leaveMessage.c_str());

            std::cout << buff_out;
            send_message(buff_out, client->uid);
            leave_flag = 1;
        } else {
            std::cout << "ERROR: -1\n";
            leave_flag = 1;
        }

        bzero(buff_out, BUFFER_SIZE);
    }

    close(client->sockfd);
    queue_remove(client->uid);
    free(client);
    client_count--;
    pthread_detach(pthread_self());

    return nullptr;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        return EXIT_FAILURE;
    }

    const char* ip = "0.0.0.0"; // IP address of the server to connect to
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd = 0; // file description
    int connfd = 0;   // connection file description
    sockaddr_in server_addr; // server address
    sockaddr_in client_addr; // socket address
    pthread_t tid;

    // Socket settings
    listenfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a Socket
    server_addr.sin_family = AF_INET;             // IPv4
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Ignore pipe signals
    signal(SIGPIPE, SIG_IGN);

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
        perror("ERROR: setsockopt failed");
        return EXIT_FAILURE;
    }

    // Bind
    if (bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR: Socket binding failed");
        return EXIT_FAILURE;
    }

    // Listen
    if (listen(listenfd, 10) < 0) {
        perror("ERROR: Socket listening failed");
        return EXIT_FAILURE;
    }

    std::cout << "Server is running on http://0.0.0.0:" << port << std::endl;

    while (1) {
        socklen_t clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clilen);

        client_t *cli = new client_t;
        cli->address = client_addr;
        cli->sockfd = connfd;
        cli->uid = uid++;

        queue_add(cli);
        pthread_create(&tid, nullptr, &handle_client, static_cast<void *>(cli));

        // Simulate some processing before accepting another connection
        sleep(1);
    }

    return EXIT_SUCCESS;
}
