#ifndef KLEE_COMMON_HPP
#define KLEE_COMMON_HPP

#include <algorithm>
#include <cstdint>
#include <limits>

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
#endif  // KLEE_COMMON_HPP
