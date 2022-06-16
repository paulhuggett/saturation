#include <iostream>

#include "sat.hpp"

int main (int argc, char const* argv[]) {
  auto const a = std::numeric_limits<int32_t>::max () - 1;
  auto const b = int32_t{2};
  auto const c = sat::adds32 (a, b);
  std::cout << std::hex << c << '\n';
}
