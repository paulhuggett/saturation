#include <iostream>

#include "saturation.hpp"

int main () {
  auto const a = std::numeric_limits<int32_t>::max () - 1;
  auto const b = int32_t{2};
  auto const c = saturation::adds32 (a, b);
  std::cout << std::hex << c << '\n';
}
