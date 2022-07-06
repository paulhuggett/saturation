#include "common.hpp"

int main () {
  using uint24 = saturation::uinteger_t<24>;
  return test_main<24, true> (
      [] (uint24 x, uint24 y) { return saturation::addu<24> (x, y); },
      [] (uint24 x, uint24 y) {
        return clampu24 (static_cast<int64_t> (x) + static_cast<int64_t> (y));
      });
}
