#ifndef SATURATION_HPP
#define SATURATION_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace saturation {

/// Yields the smallest signed integer type with at least \p Bits bits.
template <size_t Bits, typename = typename std::enable_if_t<(Bits <= 64)>>
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
template <size_t Bits, typename = typename std::enable_if_t<(Bits <= 64)>>
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

/// Equivalent to (T{1}<<N)-T{1}, where T is an unsigned integer type, but
/// without the risk of overflow if N is equal to the number of bits in T.
/// Returns 0 if n is 0.
template <size_t Bits, typename = typename std::enable_if_t<(Bits <= 64)>>
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

template <size_t Bits>
struct limits {
  using type = sinteger_t<Bits>;
  static constexpr type max () {
    return static_cast<type> ((uinteger_t<Bits>{1} << (Bits - 1U)) - 1);
  }
  static constexpr type min () { return static_cast<type> (-max () - 1); }
};

template <size_t Bits>
struct ulimits {
  using type = uinteger_t<Bits>;
  static constexpr type max () { return mask_v<Bits>; }
  static constexpr type min () { return 0U; }
};

// *******************
// unsigned arithmetic
// *******************

// addu
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr uinteger_t<Bits> addu (uinteger_t<Bits> const x,
                                 uinteger_t<Bits> const y) {
  assert (x <= ulimits<Bits>::max ());
  assert (y <= ulimits<Bits>::max ());
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
constexpr uint8_t addu8 (uint8_t const x, uint8_t const y) {
  return addu<8> (x, y);
}

// subu
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr uinteger_t<Bits> subu (uinteger_t<Bits> const x,
                                 uinteger_t<Bits> const y) {
  assert (x <= ulimits<Bits>::max ());
  assert (y <= ulimits<Bits>::max ());
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
constexpr uint8_t subu8 (uint8_t const x, uint8_t const y) {
  return subu<8> (x, y);
}

// divu
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr uinteger_t<Bits> divu (uinteger_t<Bits> const x,
                                 uinteger_t<Bits> const y) {
  assert (x <= ulimits<Bits>::max ());
  assert (y <= ulimits<Bits>::max ());
  return x / y;
}
constexpr uint32_t divu32 (uint32_t const x, uint32_t const y) {
  return divu<32> (x, y);
}
constexpr uint16_t divu16 (uint16_t const x, uint16_t const y) {
  return divu<16> (x, y);
}
constexpr uint8_t divu8 (uint8_t const x, uint8_t const y) {
  return divu<8> (x, y);
}

// mulu
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 32)>>
constexpr uinteger_t<Bits> mulu (uinteger_t<Bits> const x,
                                 uinteger_t<Bits> const y) {
  assert (x <= ulimits<Bits>::max ());
  assert (y <= ulimits<Bits>::max ());
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
constexpr uint8_t mulu8 (uint8_t const x, uint8_t const y) {
  return mulu<8> (x, y);
}

// *****************
// signed arithmetic
// *****************

namespace details {

template <size_t N, bool IsUnsigned>
class nbit_scalar {
public:
  using type = std::conditional_t<IsUnsigned, uinteger_t<N>, sinteger_t<N>>;

  constexpr nbit_scalar () : x_{0} {}
  explicit constexpr nbit_scalar (type const x) : x_{x} {}
  constexpr nbit_scalar (nbit_scalar const&) = default;
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

// adds
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr sinteger_t<Bits> adds (sinteger_t<Bits> const x,
                                 sinteger_t<Bits> const y) {
  assert (x >= limits<Bits>::min () && x <= limits<Bits>::max ());
  assert (y >= limits<Bits>::min () && y <= limits<Bits>::max ());
  using uint = uinteger_t<Bits>;
  using sint = sinteger_t<Bits>;
  using ubits = details::nbit_scalar<Bits, true>;
  using sbits = details::nbit_scalar<Bits, false>;
  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  // Get the answer (with potential overflow).
  auto const res = ubits{static_cast<uint> (ux + uy)};

  // Calculate the overflowed result as max or min depending on the sign of ux.
  auto const v = sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ux >> (Bits - 1U)) + limits<Bits>::max ())})};
  assert (v == (x < sint{0} ? limits<Bits>::min () : limits<Bits>::max ()));

  // Check for overflow.
  if (sbits{static_cast<sint> ((static_cast<uint> (v) ^ uy) | ~(uy ^ res))} >=
      0) {
    return v;
  }
  // There was no overflow: return the proper result.
  return sbits{static_cast<sint> (res)};
}
constexpr int32_t adds32 (int32_t const x, int32_t const y) {
  return adds<32> (x, y);
}
constexpr int16_t adds16 (int16_t const x, int16_t const y) {
  return adds<16> (x, y);
}
constexpr int16_t adds8 (int8_t const x, int8_t const y) {
  return adds<8> (x, y);
}

// subs
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr sinteger_t<Bits> subs (sinteger_t<Bits> const x,
                                 sinteger_t<Bits> const y) {
  assert (x >= limits<Bits>::min () && x <= limits<Bits>::max ());
  assert (y >= limits<Bits>::min () && y <= limits<Bits>::max ());
  using uint = uinteger_t<Bits>;
  using sint = sinteger_t<Bits>;
  using ubits = details::nbit_scalar<Bits, true>;
  using sbits = details::nbit_scalar<Bits, false>;

  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  auto const res = ubits{static_cast<uint> (ux - uy)};

  // Calculate the overflowed result as max or min depending on the sign of ux.
  auto const v = sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ux >> (Bits - 1U)) + limits<Bits>::max ())})};
  assert (v == (x < sint{0} ? limits<Bits>::min () : limits<Bits>::max ()));

  // Check for overflow: ux must be different from uy and res.
  if (sbits{static_cast<sint> ((ux ^ uy) & (ux ^ res))} < 0) {
    return v;
  }
  // There was no overflow: return the proper result.
  return sbits{static_cast<sint> (res)};
}
constexpr int32_t subs32 (int32_t const x, int32_t const y) {
  return subs<32> (x, y);
}
constexpr int16_t subs16 (int16_t const x, int16_t const y) {
  return subs<16> (x, y);
}
constexpr int8_t subs8 (int8_t const x, int8_t const y) {
  return subs<8> (x, y);
}

// divs
// ~~~~
// Twos complement signed division can overflow because
// limits<Bits>::min()/-1=limits<Bits>::max()+1.
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 64)>>
constexpr sinteger_t<Bits> divs (sinteger_t<Bits> const x,
                                 sinteger_t<Bits> const y) {
  assert (x >= limits<Bits>::min () && x <= limits<Bits>::max ());
  assert (y >= limits<Bits>::min () && y <= limits<Bits>::max ());
  using uint = uinteger_t<Bits>;
  using ubits = details::nbit_scalar<Bits, true>;
  return (x + !(ubits{static_cast<uint> (y + 1)} |
                ubits{static_cast<uint> (
                    static_cast<uint> (x) +
                    static_cast<uint> (limits<Bits>::min ()))})) /
         y;
}
constexpr int32_t divs32 (int32_t const x, int32_t const y) {
  return divs<32> (x, y);
}
constexpr int16_t divs16 (int16_t const x, int16_t const y) {
  return divs<16> (x, y);
}
constexpr int8_t divs8 (int8_t const x, int8_t const y) {
  return divs<8> (x, y);
}

// muls
// ~~~~
template <size_t Bits,
          typename = typename std::enable_if_t<(Bits >= 4 && Bits <= 32)>>
constexpr sinteger_t<Bits> muls (sinteger_t<Bits> const x,
                                 sinteger_t<Bits> const y) {
  assert (x >= limits<Bits>::min () && x <= limits<Bits>::max () &&
          "muls<> x value out of range");
  assert (y >= limits<Bits>::min () && y <= limits<Bits>::max () &&
          "muls<> y value out of range");

  using s2bits = details::nbit_scalar<Bits * 2, false>;
  using s2int = typename s2bits::type;

  using sbits = details::nbit_scalar<Bits, false>;
  using sint = typename sbits::type;
  using ubits = details::nbit_scalar<Bits, true>;
  using uint = typename ubits::type;

  auto const res = s2bits{
      static_cast<s2int> (static_cast<s2int> (x) * static_cast<s2int> (y))};
  if (static_cast<sint> (res >> Bits) !=
      static_cast<sint> (res >> (Bits - 1U))) {
    auto const v = sbits{static_cast<sint> (ubits{static_cast<uint> (
        static_cast<uint> (ubits{static_cast<uint> (x ^ y)} >> (Bits - 1U)) +
        static_cast<uint> (limits<Bits>::max ()))})};
    assert (v == (res < sint{0} ? limits<Bits>::min () : limits<Bits>::max ()));
    return v;
  }
  return static_cast<sint> (res);
}
constexpr int32_t muls32 (int32_t const x, int32_t const y) {
  return muls<32> (x, y);
}
constexpr int16_t muls16 (int16_t const x, int16_t const y) {
  return muls<16> (x, y);
}
constexpr int8_t muls8 (int8_t const x, int8_t const y) {
  return muls<8> (x, y);
}

}  // end namespace saturation

#endif  // SATURATION_HPP
