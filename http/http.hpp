#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "headers.hpp"
#include "types.hpp"
#include "response.hpp"

void get(routes_t& routes, const std::string& single_route, const function_pointer& func) {
  routes.push_back(route_t{ single_route, func });
}

int handle_routes(const int& client_sockfd, const routes_t& routes, const std::string& requestBuffer) {
  std::string s = "";

  for (const route_t& route : routes) {
    if (requestBuffer.find(route.route) != std::string::npos) {
      function_pointer f = route.func;
      s += (*f)();
      break;
    }
  }

  std::string response = headers(200, s.size(), "") + s;
  if (write(client_sockfd, response) != 0) {
    std::cerr << "Error writing data to socket" << "\n";
    return 1;
  }

  return 0;
}

void parse_buffer(char* buffer, const int& client_sockfd, const int& BUFFER_SIZE) {
  int bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE - 1, 0);
  if (bytes_received < 0) {
    std::cerr << "Error receiving data from client" << "\n";
    buffer[0] = '\0';
  }

  buffer[bytes_received] = '\0';
}

int accept_connection(const routes_t& routes, const int& sockfd) {
  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
  if (client_sockfd < 0) {
    std::cerr << "Error accepting incoming connection" << "\n";
    return 1;
  }

  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  parse_buffer(buffer, client_sockfd, BUFFER_SIZE);
  
  std::cout << "Request:\n" << buffer << "\n";

  handle_routes(client_sockfd, routes, buffer);

  return 0;
}

int setup_listening_socket(const routes_t& routes, const uint16_t& port) {
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
    accept_connection(routes, sockfd);
  }

  close(sockfd);
}