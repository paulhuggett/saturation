#include "common.hpp"

int main () {
  using sint = saturation::sinteger_t<24>;
  return test_main<24, false> (
      [] (sint x, sint y) { return saturation::divs<24> (x, y); },
      [] (sint x, sint y) {
        return clamp32 (static_cast<int64_t> (x) / static_cast<int64_t> (y));
      },
      false);
}
