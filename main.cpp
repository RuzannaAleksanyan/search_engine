// g++ -std=c++20 *.cpp -o search_engine -lcurl -lgumbo

#include "server.h"

int main(int argc, char **argv) {
    return start_server(argc, argv);
}