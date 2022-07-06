#include "common.hpp"

int main () {
  return test_main<32, true> (
      [] (uint32_t x, uint32_t y) { return saturation::subu32 (x, y); },
      [] (uint32_t x, uint32_t y) {
        return clampu32 (static_cast<int64_t> (x) - static_cast<int64_t> (y));
      });
}
