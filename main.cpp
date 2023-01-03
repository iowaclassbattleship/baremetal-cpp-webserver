#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "http.hpp"

int main() {
  setup_listening_socket(8080);

  return 0;
}