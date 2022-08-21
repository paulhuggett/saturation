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

inline int32_t clamp32 (int64_t const x) {
  return static_cast<int32_t> (std::clamp (
      x, static_cast<int64_t> (std::numeric_limits<int32_t>::min ()),
      static_cast<int64_t> (std::numeric_limits<int32_t>::max ())));
}

inline uint32_t clampu32 (int64_t const x) {
  return static_cast<uint32_t> (std::clamp (
      x, static_cast<int64_t> (std::numeric_limits<uint32_t>::min ()),
      static_cast<int64_t> (std::numeric_limits<uint32_t>::max ())));
}

inline uint32_t clampu32 (uint64_t const x) {
  return static_cast<uint32_t> (std::clamp (
      x, static_cast<uint64_t> (std::numeric_limits<uint32_t>::min ()),
      static_cast<uint64_t> (std::numeric_limits<uint32_t>::max ())));
}

inline uint32_t clampu24 (int64_t const x) {
  return static_cast<uint32_t> (
      std::clamp (x, int64_t{0}, static_cast<int64_t> ((1U << 24) - 1U)));
}

inline int32_t clamp24 (int64_t const x) {
  constexpr auto max = static_cast<int64_t> ((uint64_t{1} << 23) - 1);
  constexpr auto min = -max - 1;
  return static_cast<int32_t> (std::clamp (x, min, max));
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

inline void dump (char const* xn, int8_t const x, char const* yn,
                  int8_t const y) {
  std::printf ("%s=%" PRId8 " %s=%" PRId8, xn, x, yn, y);
}
inline void dump (char const* xn, uint8_t const x, char const* yn,
                  uint8_t const y) {
  std::printf ("%s=%" PRIu8 " %s=%" PRIu8, xn, x, yn, y);
}
inline void dump (char const* xn, int16_t const x, char const* yn,
                  int16_t const y) {
  std::printf ("%s=%" PRId16 " %s=%" PRId16, xn, x, yn, y);
}
inline void dump (char const* xn, uint16_t const x, char const* yn,
                  uint16_t const y) {
  std::printf ("%s=%" PRIu16 " %s=%" PRIu16, xn, x, yn, y);
}
inline void dump (char const* xn, int32_t const x, char const* yn,
                  int32_t const y) {
  std::printf ("%s=%" PRId32 " %s=%" PRId32, xn, x, yn, y);
}
inline void dump (char const* xn, uint32_t const x, char const* yn,
                  uint32_t const y) {
  std::printf ("%s=%" PRIu32 " %s=%" PRIu32, xn, x, yn, y);
}
inline void dump (char const* xn, int64_t const x, char const* yn,
                  int64_t const y) {
  std::printf ("%s=%" PRId64 " %s=%" PRId64, xn, x, yn, y);
}
inline void dump (char const* xn, uint64_t const x, char const* yn,
                  uint64_t const y) {
  std::printf ("%s=%" PRIu64 " %s=%" PRIu64, xn, x, yn, y);
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
  details::dump (namex, x, namey, y);
  std::fflush (stdout);
#endif
  type const actual = fut (x, y);
  type const e = expected (x, y);
#if KLEE_RUN
  details::dump (" expected", e, "actual", actual);
  std::putchar ('\n');
  std::fflush (stdout);
#endif
  return actual == e ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif  // KLEE_COMMON_HPP
