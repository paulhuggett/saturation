#include <klee/klee.h>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include "common.hpp"
#include "saturation.hpp"

static uint32_t sat_addu24_branchy (uint32_t const x, uint32_t const y) {
  return clampu24 (static_cast<int64_t> (x) + static_cast<int64_t> (y));
}

int main () {
  auto a = uint32_t{0};
  auto b = uint32_t{0};
  klee_make_symbolic (&a, sizeof (a), "a");
  klee_make_symbolic (&b, sizeof (b), "b");
  klee_assume (a <= (1 << 23));
  klee_assume (b <= (1 << 23));

  uint32_t const c = sat::addu<24> (a, b);
  uint32_t const expected = sat_addu24_branchy (a, b);
#if KLEE_RUN
  std::printf ("a=%" PRIu32 " b=%" PRIu32 " expected=%" PRIu32
               " actual=%" PRIu32 "\n",
               a, b, expected, c);
#endif
  return c == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
