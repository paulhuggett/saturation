#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include "common.hpp"

using sint = saturation::sinteger_t<24>;

static int32_t sat_subs24_branchy (sint const x, sint const y) {
  return clamp24 (static_cast<int64_t> (x) - static_cast<int64_t> (y));
}

int main () {
  auto a = sint{0};
  auto b = sint{0};
  klee_make_symbolic (&a, sizeof (a), "a");
  klee_make_symbolic (&b, sizeof (b), "b");
  klee_assume_range<24> (a);
  klee_assume_range<24> (b);

  auto const c = saturation::subs<24> (a, b);
  auto const expected = sat_subs24_branchy (a, b);
#if KLEE_RUN
  std::printf ("a=%" PRId32 " b=%" PRId32 " expected=%" PRId32
               " actual=%" PRId32 "\n",
               a, b, expected, c);
#endif
  return c == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
