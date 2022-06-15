#ifndef SAT_HPP
#define SAT_HPP

#include <cstdint>
#include <limits>

namespace sat {

// *******************    
// unsigned arithmetic
// *******************

constexpr uint32_t addu32(uint32_t x, uint32_t y) {
  uint32_t res = x + y;
  res |= -(res < x);
  return res;
}

constexpr uint32_t subu32(uint32_t x, uint32_t y) {
  uint32_t res = x - y;
  res &= -(res <= x);
  return res;
}

constexpr uint32_t divu32(uint32_t x, uint32_t y) {
  return x / y;
}

constexpr uint32_t mulu32(uint32_t x, uint32_t y) {
  uint64_t res = static_cast<uint64_t> (x) * static_cast<uint64_t> (y);
  uint32_t const hi = res >> 32;
  auto const lo = static_cast<uint32_t> (res);
  return lo | -!!hi;
}

// *****************
// signed arithmetic
// *****************

constexpr int32_t adds32(int32_t const x, int32_t const y) {
  auto ux = static_cast<uint32_t> (x);
  auto const uy = static_cast<uint32_t> (y);
  uint32_t const res = ux + uy;

  // Calculate overflowed result but don't change the sign bit of ux.
  ux = (ux >> 31) + std::numeric_limits<int32_t>::max ();
  if (static_cast<int32_t> ((ux ^ uy) | ~(uy ^ res)) >= 0) {
      return static_cast<int32_t> (ux);
  }
  return static_cast<int32_t> (res);
}

constexpr int32_t subs32(int32_t const x, int32_t const y) {
  uint32_t ux = static_cast<uint32_t> (x);
  uint32_t const uy = static_cast<uint32_t> (y);
  uint32_t const res = ux - uy;
  ux = (ux >> 31) + std::numeric_limits<int32_t>::max();
  if (static_cast<int32_t>((ux ^ uy) & (ux ^ res)) < 0) {
    return static_cast<int32_t> (ux);
  }
  return static_cast<int32_t> (res);
}

// Division, unlike in the unsigned case, can overflow in signed arithmetic. The only case where it happens is when you divide INT_MIN by -1 since the correct answer would be INT_MAX + 1. Another problem is that if we try to do the division with these arguments an exception will be raised. Thus we cannot use the same technique as in addition and subtraction where we correct the overflow after it has occurred.

// We instead need to alter the arguments in the special case where they would otherwise overflow. This isn't that hard: by adding one to the argument that starts as INT_MIN we produce the correct answer. Thus the algorithm needs to quickly check for the special case. We then add the true/false (1 or 0) Boolean result to the dividend to prevent overflow:


constexpr int32_t divs32_branchy(int32_t x, int32_t y) {
    // Only min/-1 can overflow.
    if (x == std::numeric_limits<int32_t>::min () && y == -1) {
        return std::numeric_limits<int32_t>::max ();
    }
    return x / y;
}

constexpr int32_t divs32(int32_t x, int32_t y) {
    // Only min/-1 can overflow.
    x += !((y + 1) | ((uint32_t) x + std::numeric_limits<int32_t>::min ()));
    return x / y;
}

constexpr int32_t muls32(int32_t const x, int32_t const y) {
  int64_t res = static_cast<int64_t> (x) * static_cast<int64_t> (y);
  uint32_t res2 = (static_cast<uint32_t> (x ^ y) >> 31) + std::numeric_limits<int32_t>::max ();

  int32_t hi = (res >> 32);
  auto const lo = static_cast<int32_t> (res);
  if (hi != (lo >> 31)) {
    res = res2;
  }
  return static_cast<int32_t> (res);
}

} // end namespace sat

#endif // SAT_HPP
