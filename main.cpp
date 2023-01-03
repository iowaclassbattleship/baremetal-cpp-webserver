#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <map>

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
  std::map<std::string, function_pointer> routes;
  get(routes, "/", handle_index);
  post(routes, "/", handle_post);

  setup_listening_socket(routes, 8080);

  return 0;
}