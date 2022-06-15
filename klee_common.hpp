#ifndef KLEE_COMMON_HPP
#define KLEE_COMMON_HPP

#include <algorithm>
#include <cstdint>
#include <limits>

#ifndef KLEE_RUN
#define KLEE_RUN 0
#endif

inline int32_t clamp32 (int64_t const x) {
  return static_cast<int32_t> (std::clamp (x,
                                             static_cast<int64_t> (std::numeric_limits<int32_t>::min ()),
                                             static_cast<int64_t> (std::numeric_limits<int32_t>::max ())));
}

#endif // KLEE_COMMON_HPP
