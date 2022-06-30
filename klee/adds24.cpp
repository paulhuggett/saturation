#include <klee/klee.h>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include "common.hpp"
#include "saturation.hpp"

static int32_t sat_adds24_branchy (int32_t const x, int32_t const y) {
  return clamp24 (static_cast<int64_t> (x) + static_cast<int64_t> (y));
}

int main () {
  auto a = int32_t{0};
  auto b = int32_t{0};
  klee_make_symbolic (&a, sizeof (a), "a");
  klee_make_symbolic (&b, sizeof (b), "b");

  klee_assume (a <= sat::limits<24>::max ());
  klee_assume (a >= sat::limits<24>::min ());
  klee_assume (b <= sat::limits<24>::max ());
  klee_assume (b >= sat::limits<24>::min ());

  int32_t const c = sat::adds<24> (a, b);
  int32_t const expected = sat_adds24_branchy (a, b);
#if KLEE_RUN
  std::printf ("a=%" PRId32 " b=%" PRId32 " expected=%" PRId32
               " actual=%" PRId32 "\n",
               a, b, expected, c);
#endif
  return c == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
