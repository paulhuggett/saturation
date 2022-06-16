#include <klee/klee.h>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include "common.hpp"
#include "sat.hpp"

static int32_t sat_muls32_branchy (int32_t const x, int32_t const y) {
  return clamp32 (static_cast<int64_t> (x) * static_cast<int64_t> (y));
}

int main () {
  auto a = int32_t{0};
  auto b = int32_t{0};
  klee_make_symbolic (&a, sizeof (a), "a");
  klee_make_symbolic (&b, sizeof (b), "b");

  int32_t const c = sat::muls32 (a, b);
  int32_t const expected = sat_muls32_branchy (a, b);
#if KLEE_RUN
  std::printf ("a=%" PRId32 " b=%" PRId32 " expected=%" PRId32
               " actual=%" PRId32 "\n",
               a, b, expected, c);
#endif
  return c == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
