#ifndef KLEE_COMMON_HPP
#define KLEE_COMMON_HPP

#include <klee/klee.h>

#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

#include "saturation/saturation.hpp"

#ifndef KLEE_RUN
#define KLEE_RUN 0
#endif

template <size_t N, typename T>
saturation::sinteger_t<N> clamps (T z) {
  constexpr auto max = saturation::slimits<N>::max ();
  if (z > max) {
    z = max;
  }
  constexpr auto min = saturation::slimits<N>::min ();
  if (z < min) {
    z = min;
  }
  return static_cast<saturation::sinteger_t<N>> (z);
}

template <size_t N, typename T>
saturation::uinteger_t<N> clampu (T z) {
  constexpr auto max = saturation::ulimits<N>::max ();
  if (z > max) {
    z = max;
  }
  return static_cast<saturation::uinteger_t<N>> (z);
}

template <size_t Bits, bool IsUnsigned>
using test_type = std::conditional_t<IsUnsigned, saturation::uinteger_t<Bits>,
                                     saturation::sinteger_t<Bits>>;

namespace details {

template <size_t Bits, bool IsUnsigned>
struct ar {
  static inline void assume (test_type<Bits, IsUnsigned> x) {
    klee_assume (x >= saturation::slimits<Bits>::min ());
    klee_assume (x <= saturation::slimits<Bits>::max ());
  }
};

template <size_t Bits>
struct ar<Bits, true> {
  static inline void assume (test_type<Bits, true> x) {
    klee_assume (x >= saturation::ulimits<Bits>::min ());
    klee_assume (x <= saturation::ulimits<Bits>::max ());
  }
};

template <size_t Bits, bool IsUnsigned>
inline void assume_range (test_type<Bits, IsUnsigned> v) {
  ar<Bits, IsUnsigned>::assume (v);
}

inline void dump (std::pair<char const *, int8_t> const x,
                  std::pair<char const *, int8_t> const y) {
  std::printf ("%s=%" PRId8 " %s=%" PRId8, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, uint8_t> const x,
                  std::pair<char const *, uint8_t> const y) {
  std::printf ("%s=%" PRIu8 " %s=%" PRIu8, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, int16_t> const x,
                  std::pair<char const *, int16_t> const y) {
  std::printf ("%s=%" PRId16 " %s=%" PRId16, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, uint16_t> const x,
                  std::pair<char const *, uint16_t> const y) {
  std::printf ("%s=%" PRIu16 " %s=%" PRIu16, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, int32_t> x,
                  std::pair<char const *, int32_t> const y) {
  std::printf ("%s=%" PRId32 " %s=%" PRId32, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, uint32_t> const x,
                  std::pair<char const *, uint32_t> const y) {
  std::printf ("%s=%" PRIu32 " %s=%" PRIu32, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, int64_t> const x,
                  std::pair<char const *, int64_t> const y) {
  std::printf ("%s=%" PRId64 " %s=%" PRId64, x.first, x.second, y.first,
               y.second);
}
inline void dump (std::pair<char const *, uint64_t> const x,
                  std::pair<char const *, uint64_t> const y) {
  std::printf ("%s=%" PRIu64 " %s=%" PRIu64, x.first, x.second, y.first,
               y.second);
}

}  // end namespace details

/// \tparam Bits  The number of bits in the type to be tested.
/// \tparam IsUnsigned  True if the test is to be performed with an unsigned
///   integer type, false for a signed type.
/// \tparam FunctionUnderTest  A function with signature compatible with T(T,T)
///   where T is an integer type of width Bits and whose sign is given by
///   IsUnsigned. It should invoke the function under test with the supplied
///   argument and return its result.
/// \tparam ExpectedFunction  A function with signature compatible with T(T,T)
///   where T is an integer type of width Bits and whose sign is given by
///   IsUnsigned. It should return the result expected from the function under
///   test.
/// \param fut  A function which will invoke the function under test.
/// \param expected  A function which will yield the test's expected result.
/// \param allow_y0  Is the second argument passed to \p fut or \p expected
///   permitted to have the value 0? Normally true, but set false when testing
///   division operations to avoid division by zero.
/// \returns EXIT_SUCCESS if the function under test produces the expected
///   result, EXIT_FAILURE otherwise.
template <size_t Bits, bool IsUnsigned, typename FunctionUnderTest,
          typename ExpectedFunction>
int test_main (FunctionUnderTest fut, ExpectedFunction expected,
               bool const allow_y0 = true) {
  using type = test_type<Bits, IsUnsigned>;

  auto x = type{0};
  auto y = type{0};
  char namex[4];
  std::snprintf (namex, sizeof (namex), "x%zu", Bits);
  klee_make_symbolic (&x, sizeof (x), namex);
  char namey[4];
  std::snprintf (namey, sizeof (namey), "y%zu", Bits);
  klee_make_symbolic (&y, sizeof (y), namey);
  details::assume_range<Bits, IsUnsigned> (x);
  details::assume_range<Bits, IsUnsigned> (y);
  if (!allow_y0) {
    klee_assume (y != 0);
  }

#if KLEE_RUN
  details::dump (std::pair<char const *, type> (namex, x),
                 std::pair<char const *, type> (namey, y));
  std::fflush (stdout);
#endif

  type const actual = fut (x, y);
  type const e = expected (x, y);

#if KLEE_RUN
  details::dump (std::make_pair (" expected", e),
                 std::make_pair ("actual", actual));
  std::putchar ('\n');
  std::fflush (stdout);
#endif
  return actual == e ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif  // KLEE_COMMON_HPP
