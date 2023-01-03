#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <iostream>

int write(const int& client_sockfd, const std::string& response) {
  if (send(client_sockfd, response.c_str(), response.size(), 0) == -1) {
    std::cerr << "Error sending data" << "\n";
    return 1;
  }

  if (close(client_sockfd) == -1) {
    std::cerr << "Error closing socket" << "\n";
    return 1;
  }

  return 0;
}