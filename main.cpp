#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <vector>

#include "http/http.hpp"

std::string get_file_as_string(std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << "\n";
        exit(1);
    }

    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>()) + "\n\0";
}

void handle_index(const int& client_sockfd) {
  std::string filename = "static/index.html";
  std::string res = get_file_as_string(filename);

  std::string response = headers(200, res.size(), "html") + res;
  write(client_sockfd, response);
}

void handle_post(const int& client_sockfd) {
  std::string filename = "static/styles.css";
  std::string res = get_file_as_string(filename);

  std::string response = headers(200, res.size(), "css") + res;
  write(client_sockfd, response);
}

int main() {
  routes_t routes;
  get(routes, "/test", handle_post);
  get(routes, "/", handle_index);

  setup_listening_socket(routes, 8080);

  return 0;
}