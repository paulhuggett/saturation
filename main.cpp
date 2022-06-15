#include <iostream>

#include "sat.hpp"

int main(int argc, const char * argv[]) {
  int32_t const a = std::numeric_limits<int32_t>::max () - 1;
  int32_t const b = int32_t{2};
  int32_t const c = sat::adds32(a, b);
  std::cout << std::hex << c << '\n';
}
