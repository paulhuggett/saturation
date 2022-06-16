#include <klee/klee.h>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include "common.hpp"
#include "saturation.hpp"

static uint32_t sat_divu32_branchy (uint32_t const x, uint32_t const y) {
  return clampu32 (static_cast<uint64_t> (x) / static_cast<uint64_t> (y));
}

int main () {
  auto a = uint32_t{0};
  auto b = uint32_t{0};
  klee_make_symbolic (&a, sizeof (a), "a");
  klee_make_symbolic (&b, sizeof (b), "b");
  klee_assume (b != 0);

  uint32_t const c = sat::divu32 (a, b);
  uint32_t const expected = sat_divu32_branchy (a, b);
#if KLEE_RUN
  std::printf ("a=%" PRIu32 " b=%" PRIu32 " expected=%" PRIu32
               " actual=%" PRIu32 "\n",
               a, b, expected, c);
#endif
  return c == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
