#ifndef SATURATION_HPP
#define SATURATION_HPP

#include <cstdint>
#include <limits>

namespace sat {

/// Yields the smallest unsigned integer type with at least \p Bits bits.
template <size_t Bits>
struct uinteger {
  using type = typename uinteger<Bits + 1>::type;
};
template <size_t Bits>
using uinteger_t = typename uinteger<Bits>::type;
template <>
struct uinteger<8> {
  using type = uint8_t;
};
template <>
struct uinteger<16> {
  using type = uint16_t;
};
template <>
struct uinteger<32> {
  using type = uint32_t;
};
template <>
struct uinteger<64> {
  using type = uint64_t;
};
// Equivalent to (T{1}<<N)-T{1}, where T is an unsigned integer type, but
// without the risk of overflow if N is equal to the number of bits in T.
// Returns 0 if n is 0.
template <unsigned Bits>
struct mask {
  static constexpr uinteger_t<Bits> value =
      uinteger_t<Bits>{mask<Bits - 1U>::value} << 1U | uinteger_t<Bits>{1};
};
template <>
struct mask<0U> {
  static constexpr uinteger_t<1U> value = 0U;
};
template <unsigned Bits>
inline constexpr auto mask_v = mask<Bits>::value;

// *******************
// unsigned arithmetic
// *******************

template <unsigned Bits>
constexpr uinteger_t<Bits> addu (uinteger_t<Bits> const x, uinteger_t<Bits> const y) {
  constexpr auto maxu = mask_v<Bits>;
  uinteger_t<Bits> res = x + y;
  res |= -((res < x) | (res > maxu));
  return res & maxu;
}
constexpr uint32_t addu32 (uint32_t const x, uint32_t const y) {
  return addu<32> (x, y);
}
constexpr uint16_t addu16 (uint16_t const x, uint16_t const y) {
  return addu<16> (x, y);
}

template <unsigned Bits>
constexpr uinteger_t<Bits> subu (uinteger_t<Bits> const x, uinteger_t<Bits> const y) {
  constexpr auto maxu = mask_v<Bits>;
  uinteger_t<Bits> res = x - y;
  res &= -(res <= x);
  res &= maxu;
  return res;
}
constexpr uint32_t subu32 (uint32_t const x, uint32_t const y) {
  return subu<32> (x, y);
}
constexpr uint16_t subu16 (uint16_t const x, uint16_t const y) {
  return subu<16> (x, y);
}

template <unsigned Bits>
constexpr uinteger_t<Bits> divu (uinteger_t<Bits> const x, uinteger_t<Bits> const y) {
  return x / y;
}
constexpr uint32_t divu32 (uint32_t const x, uint32_t const y) {
  return divu<32> (x, y);
}
constexpr uint16_t divu16 (uint16_t const x, uint16_t const y) {
  return divu<16> (x, y);
}

template <unsigned Bits>
constexpr uinteger_t<Bits> mulu (uinteger_t<Bits> const x, uinteger_t<Bits> const y) {
  auto res =
      static_cast<uinteger_t<Bits * 2U>> (x) * static_cast<uinteger_t<Bits * 2U>> (y);
  auto const hi = res >> Bits;
  auto const lo = static_cast<uinteger_t<Bits>> (res);
  return (lo | -!!hi) & mask_v<Bits>;
}
constexpr uint32_t mulu32 (uint32_t x, uint32_t y) {
  return mulu<32> (x, y);
}

// *****************
// signed arithmetic
// *****************

constexpr int32_t adds32 (int32_t const x, int32_t const y) {
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

constexpr int32_t subs32 (int32_t const x, int32_t const y) {
  auto ux = static_cast<uint32_t> (x);
  auto const uy = static_cast<uint32_t> (y);
  uint32_t const res = ux - uy;
  ux = (ux >> 31) + std::numeric_limits<int32_t>::max ();
  if (static_cast<int32_t> ((ux ^ uy) & (ux ^ res)) < 0) {
    return static_cast<int32_t> (ux);
  }
  return static_cast<int32_t> (res);
}

// Division, unlike in the unsigned case, can overflow in signed arithmetic. The
// only case where it happens is when you divide INT_MIN by -1 since the correct
// answer would be INT_MAX + 1.
constexpr int32_t divs32 (int32_t x, int32_t y) {
  x += !(static_cast<uint32_t> (y + 1) |
         (static_cast<uint32_t> (x) +
          static_cast<uint32_t> (std::numeric_limits<int32_t>::min ())));
  return x / y;
}

constexpr int32_t muls32 (int32_t const x, int32_t const y) {
  auto const res = static_cast<int64_t> (x) * static_cast<int64_t> (y);
  if (static_cast<int32_t> (res >> 32) != (static_cast<int32_t> (res) >> 31)) {
    return (static_cast<uint32_t> (x ^ y) >> 31) +
           std::numeric_limits<int32_t>::max ();
  }
  return static_cast<int32_t> (res);
}

}  // end namespace sat

#endif  // SATURATION_HPP
