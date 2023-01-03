#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include "http_headers.hpp"

enum Methods { GET, POST };

std::string get_file_as_string(std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << "\n";
        exit(1);
    }

    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>()) + "\n\0";
}

int accept_connection(int sockfd) {
  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
  if (client_sockfd < 0) {
    std::cerr << "Error accepting incoming connection" << "\n";
    return 1;
  }

  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  int bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE - 1, 0);
  if (bytes_received < 0) {
    std::cerr << "Error receiving data from client" << "\n";
    return 1;
  }

  buffer[bytes_received] = '\0';
  std::cout << "Request:\n" << buffer << "\n";

  std::string filename = "index.html";
  std::string content = get_file_as_string(filename);

  std::string response = headers(content.size()) + content;

  if (send(client_sockfd, response.c_str(), response.size(), 0) == -1) {
    std::cerr << "Error sending data" << "\n";
    return 1;
  }

  if (close(client_sockfd) == -1) {
    std::cerr << "Error closing socket" << "\n";
    return 1;
  }

  std::cout << "Response:\n" << response;

  close(client_sockfd);

  return 0;
}

int setup_listening_socket(uint16_t port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error opening socket" << "\n";
    return 1;
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    std::cerr << "Error binding socket to port 8080" << "\n";
    return 1;
  }

  if (listen(sockfd, 5) < 0) {
    std::cerr << "Error listening on socket" << "\n";
    return 1;
  }

  while (true) {
    accept_connection(sockfd);
  }

  close(sockfd);
}