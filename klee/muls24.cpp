#include "common.hpp"

int main () {
  constexpr size_t bits = 24;
  using sint = saturation::sinteger_t<bits>;
  return test_main<bits, false> (
      [] (sint x, sint y) { return saturation::muls<bits> (x, y); },
      [] (sint x, sint y) {
        return clamp32 (static_cast<int64_t> (x) * static_cast<int64_t> (y));
      });
}
