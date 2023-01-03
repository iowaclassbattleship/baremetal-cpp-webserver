#include <string>
#include <vector>

using function_pointer = std::string (*)();
using route_t = struct {
  std::string route;
  function_pointer func;
};
using routes_t = std::vector<route_t>;