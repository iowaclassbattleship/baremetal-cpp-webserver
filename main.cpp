#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "http/http.hpp"

std::string handle_index() {
  std::string filename = "static/index.html";

  return get_file_as_string(filename);
}

std::string handle_post() {
  std::string filename = "static/test.html";

  return get_file_as_string(filename);
}

int main() {
  routes rs;
  get(rs, "/test", handle_post);
  get(rs, "/", handle_index);

  setup_listening_socket(rs, 8080);

  return 0;
}