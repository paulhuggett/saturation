#ifndef SATURATION_HPP
#define SATURATION_HPP

#include <cstdint>
#include <limits>

namespace sat {

/// Yields the smallest signed integer type with at least \p Bits bits.
template <size_t Bits>
struct sinteger {
  using type = typename sinteger<Bits + 1>::type;
};
template <size_t Bits>
using sinteger_t = typename sinteger<Bits>::type;
template <>
struct sinteger<8> {
  using type = int8_t;
};
template <>
struct sinteger<16> {
  using type = int16_t;
};
template <>
struct sinteger<32> {
  using type = int32_t;
};
template <>
struct sinteger<64> {
  using type = int64_t;
};

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
  auto const res = static_cast<uinteger_t<Bits * 2U>> (x) *
                   static_cast<uinteger_t<Bits * 2U>> (y);
  auto const hi = res >> Bits;
  auto const lo = static_cast<uinteger_t<Bits>> (res);
  return (lo | -!!hi) & mask_v<Bits>;
}
constexpr uint32_t mulu32 (uint32_t const x, uint32_t const y) {
  return mulu<32> (x, y);
}
constexpr uint16_t mulu16 (uint16_t const x, uint16_t const y) {
  return mulu<16> (x, y);
}

// *****************
// signed arithmetic
// *****************

template <size_t Bits>
struct limits {
  using type = sat::sinteger_t<Bits>;
  static constexpr type max () {
    return static_cast<type> ((sat::uinteger_t<Bits>{1} << (Bits - 1U)) - 1);
  }
  static constexpr type min () { return static_cast<type> (-max () - 1); }
};

template <size_t Bits>
struct ulimits {
  using type = sat::uinteger_t<Bits>;
  static constexpr type max () { return mask_v<Bits>; }
  static constexpr type min () { return 0U; }
};

constexpr int32_t adds32 (int32_t const x, int32_t const y) {
  auto const ux = static_cast<uint32_t> (x);
  auto const uy = static_cast<uint32_t> (y);
  uint32_t const res = ux + uy;

  // Calculate overflowed result but don't change the sign bit of ux.
  auto const vres = (ux >> 31) + std::numeric_limits<int32_t>::max ();
  assert (vres == limits<32>::max () || vres == limits<32>::min ());
  if (static_cast<int32_t> ((vres ^ uy) | ~(uy ^ res)) >= 0) {
    return static_cast<int32_t> (vres);
  }
  return static_cast<int32_t> (res);
}

namespace details {

template <unsigned N, bool IsUnsigned>
class nbit_scalar {
public:
  using type = std::conditional_t<IsUnsigned, uinteger_t<N>, sinteger_t<N>>;

  constexpr nbit_scalar () : x_{0} {}
  explicit constexpr nbit_scalar (type const a) : x_{a} {}
  constexpr nbit_scalar (nbit_scalar const& rhs) = default;
  nbit_scalar& operator= (type const x) {
    x_ = x;
    return *this;
  }
  nbit_scalar& operator= (nbit_scalar const&) = default;

  constexpr operator type () const { return x_; }

private:
  type x_ : N;
};

}  // end namespace details

template <unsigned Bits>
constexpr sinteger_t<Bits> adds (sinteger_t<Bits> const x,
                                 sinteger_t<Bits> const y) {
  using uint = uinteger_t<Bits>;
  using sint = sinteger_t<Bits>;
  using ubits = details::nbit_scalar<Bits, true>;
  using sbits = details::nbit_scalar<Bits, false>;
  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  // Get the answer (with potential overflow).
  auto const res = ubits{static_cast<uint> (ux + uy)};

  // Calculate the overflowed result but preserve the sign of ux.
  auto const vress = sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ux >> (Bits - 1U)) + limits<Bits>::max ())})};
  assert (vress == limits<Bits>::max () || vress == limits<Bits>::min ());
  if (sbits{static_cast<sint> (ubits{static_cast<uint> (ux ^ uy)} |
                               ubits{static_cast<uint> (~(uy ^ res))})} >= 0) {
    return vress;
  }
  return static_cast<sinteger_t<Bits>> (res);
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