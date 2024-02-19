#include "server.h"

#define MAX_CLIENTS 100
#define BUFFER_SIZE 2048

std::atomic<unsigned int> client_count{0};
int uid = 10;

client_t *clients[MAX_CLIENTS];
std::mutex clients_mutex;

void queue_add(client_t *cl) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
}

// void send_message(const char *s, int uid) {
//     std::lock_guard<std::mutex> lock(clients_mutex);

//     for (int i = 0; i < MAX_CLIENTS; ++i) {
//         if (clients[i]) {
//             if (clients[i]->uid != uid) {
//                 if (write(clients[i]->sockfd, s, strlen(s)) < 0) {
//                     perror("ERROR: write to descriptor failed");
//                     break;
//                 }
//             }
//         }
//     }
// }

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

std::string get_last_line(const std::string& text) {
    std::istringstream iss(text);
    std::string last_line;

    for (std::string line; std::getline(iss, line); ) {
        if (!line.empty())
            last_line = line;
    }

    return last_line;
}

void* handle_client(void *arg) {
    char buff_out[BUFFER_SIZE];
    int leave_flag = 0;

    client_count++;
    client_t *client = static_cast<client_t *>(arg);

    // Initialize name directly
    std::ostringstream oss;
    oss << "Client_" << client->uid;
    strcpy(client->name, oss.str().c_str());

    std::memset(buff_out, 0, BUFFER_SIZE);

    while (1) {
        if (leave_flag) {
            break;
        }

        int receive = recv(client->sockfd, buff_out, BUFFER_SIZE, 0);

        if (receive > 0) {
            if (strlen(buff_out) > 0) {
                // send_message(buff_out, client->uid);
                
                std::string message;
                message = get_last_line(buff_out);
                str_trim_lf(buff_out, strlen(buff_out));

                start_search_engine(message);

            }
        } else if (receive == 0 || strcmp(buff_out, "exit") == 0) {
            std::ostringstream oss;
            oss << client->name << " has left\n";
            std::string leaveMessage = oss.str();
            strcpy(buff_out, leaveMessage.c_str());

            // send_message(buff_out, client->uid);
            leave_flag = 1;
        } else {
            std::cout << "ERROR: -1\n";
            leave_flag = 1;
        }

        std::memset(buff_out, 0, BUFFER_SIZE);
    }

    close(client->sockfd);
    queue_remove(client->uid);
    free(client);
    client_count--;
    pthread_detach(pthread_self());

    return nullptr;
}

int start_server(int argc, char **argv) {
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

    //std::cout << "Server is running on http://0.0.0.0:" << port << std::endl;
    std::cout << "asdsda";
    while (1) {
        socklen_t clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clilen);

        client_t *cli = new client_t;
        cli->address = client_addr;
        cli->sockfd = connfd;
        cli->uid = uid++;

        queue_add(cli);
        pthread_create(&tid, nullptr, &handle_client, static_cast<void *>(cli));

        sleep(1);
    }

    return EXIT_SUCCESS;
}