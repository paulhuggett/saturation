#include "common.hpp"

int main () {
  return test_main<32, false> (
      [] (int32_t x, int32_t y) { return saturation::subs<32> (x, y); },
      [] (int32_t x, int32_t y) {
        return clamp32 (static_cast<int64_t> (x) - static_cast<int64_t> (y));
      });
}
