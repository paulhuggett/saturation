#ifndef KLEE_COMMON_HPP
#define KLEE_COMMON_HPP

#include <klee/klee.h>

#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

#include "saturation.hpp"

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
    klee_assume (x >= saturation::limits<Bits>::min ());
    klee_assume (x <= saturation::limits<Bits>::max ());
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

inline void dump (int8_t x, int8_t y, int8_t expected, int8_t actual) {
  std::printf ("x=%" PRId8 " y=%" PRId8 " expected=%" PRId8 " actual=%" PRId8
               "\n",
               x, y, expected, actual);
}
inline void dump (uint8_t x, uint8_t y, uint8_t expected, uint8_t actual) {
  std::printf ("x=%" PRIu8 " y=%" PRIu8 " expected=%" PRIu8 " actual=%" PRIu8
               "\n",
               x, y, expected, actual);
}
inline void dump (int16_t x, int16_t y, int16_t expected, int16_t actual) {
  std::printf ("x=%" PRId16 " y=%" PRId16 " expected=%" PRId16
               " actual=%" PRId16 "\n",
               x, y, expected, actual);
}
inline void dump (uint16_t x, uint16_t y, uint16_t expected, uint16_t actual) {
  std::printf ("x=%" PRIu16 " y=%" PRIu16 " expected=%" PRIu16
               " actual=%" PRIu16 "\n",
               x, y, expected, actual);
}
inline void dump (int32_t x, int32_t y, int32_t expected, int32_t actual) {
  std::printf ("x=%" PRId32 " y=%" PRId32 " expected=%" PRId32
               " actual=%" PRId32 "\n",
               x, y, expected, actual);
}
inline void dump (uint32_t x, uint32_t y, uint32_t expected, uint32_t actual) {
  std::printf ("x=%" PRIu32 " y=%" PRIu32 " expected=%" PRIu32
               " actual=%" PRIu32 "\n",
               x, y, expected, actual);
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
  klee_make_symbolic (&x, sizeof (x), "x");
  klee_make_symbolic (&y, sizeof (y), "y");
  details::assume_range<Bits, IsUnsigned> (x);
  details::assume_range<Bits, IsUnsigned> (y);
  if (!allow_y0) {
    klee_assume (y != 0);
  }

  type const actual = fut (x, y);
  type const e = expected (x, y);
#if KLEE_RUN
  details::dump (x, y, e, actual);
#endif
  return actual == e ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif  // KLEE_COMMON_HPP
