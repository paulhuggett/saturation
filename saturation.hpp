/// \file saturation.hpp
/// \brief A collection of functions which provide saturating arithmetic
/// (addition, subtraction, multiplication, and division) for signed and
/// unsigned integer types.
///
/// Types can range from 4 to 64 bits for addition, subtraction, and division,
/// and from 4 to 32 bits for multiplication. Operations using integers of width
/// matching target registers are likely to be branchless.

#ifndef SATURATION_HPP
#define SATURATION_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

#ifndef HAVE_INT128
#define HAVE_INT128  0
#endif

namespace saturation {

/// Yields the smallest signed integral type with at least \p N bits.
template <size_t N, typename = typename std::enable_if_t<(N <= 128)>>
struct sinteger {
  /// The type of a signed integral with at least \p N bits.
  using type = typename sinteger<N + 1>::type;
};
/// \brief A helper type for convenient use of sinteger<N>::type.
template <size_t N>
using sinteger_t = typename sinteger<N>::type;
/// \brief Yields a signed integral type of 8 bits or more.
template <>
struct sinteger<8> {
  /// Smallest signed integer type with width of at least 8 bits.
  using type = int_least8_t;
};
/// \brief Yields a signed integral type of 16 bits or more.
template <>
struct sinteger<16> {
  /// Smallest signed integer type with width of at least 16 bits.
  using type = int_least16_t;
};
/// \brief Yields a signed integral type of 32 bits or more.
template <>
struct sinteger<32> {
  /// Smallest signed integer type with width of at least 32 bits.
  using type = int_least32_t;
};
/// \brief Yields a signed integral type of 64 bits or more.
template <>
struct sinteger<64> {
  /// Smallest signed integer type with width of at least 64 bits.
  using type = int_least64_t;
};
#if HAVE_INT128
/// \brief Yields a signed integral type of 128 bits.
template <>
struct sinteger<128> {
  /// A 128 bit signed integer type.
  using type = __int128;
};
#else
/// \brief Yields no type when the compiler does not support 128 bit integers.
template <>
struct sinteger<128> {};
#endif

/// \brief Yields the smallest unsigned integral type with at least \p N bits.
template <size_t N, typename = typename std::enable_if_t<(N <= 128)>>
struct uinteger {
  /// The type of an unsigned integral with at least \p N bits.
  using type = typename uinteger<N + 1>::type;
};
/// \brief A helper type for convenient use of uinteger<N>::type.
template <size_t N>
using uinteger_t = typename uinteger<N>::type;
/// \brief Yields an unsigned integral type of 8 bits or more.
template <>
struct uinteger<8> {
  /// Smallest unsigned integer type with width of at least 8 bits.
  using type = uint_least8_t;
};
/// \brief Yields an unsigned integral type of 16 bits or more.
template <>
struct uinteger<16> {
  /// Smallest unsigned integer type with width of at least 16 bits.
  using type = uint_least16_t;
};
/// \brief Yields an unsigned integral type of 32 bits or more.
template <>
struct uinteger<32> {
  /// Smallest unsigned integer type with width of at least 32 bits.
  using type = uint_least32_t;
};
/// \brief Yields an unsigned integral type of 64 bits or more.
template <>
struct uinteger<64> {
  /// Smallest unsigned integer type with width of at least 64 bits.
  using type = uint_least64_t;
};
#if HAVE_INT128
/// \brief Yields an unsigned integral type of 128 bits.
template <>
struct uinteger<128> {
  /// A 128 bit signed integer type.
  using type = unsigned __int128;
};
#else
/// \brief Yields no type when the compiler does not support 128 bit integers.
template <>
struct uinteger<128> {};
#endif

/// \brief Equivalent to (T{1}<<N)-T{1} where T is an unsigned integral type.
///
/// Avoids the risk of overflow if N is equal to the number of bits in T.
/// Yields 0 if \p N is 0.
///
/// \tparam N  The number of bits of result.
template <size_t N, typename = typename std::enable_if_t<(N <= 64)>>
struct mask {
  /// Static constant of type uinteger_t<N> with value \f$ 2^N-1 \f$.
  static constexpr uinteger_t<N> value =
      uinteger_t<N>{mask<N - 1U>::value} << 1U | uinteger_t<N>{1};
};
/// \brief mask<0> is defined to be 0.
template <>
struct mask<0U> {
  /// Static constant of type uinteger_t<1> with value 0.
  static constexpr uinteger_t<1U> value = 0U;
};
/// \brief A helper variable template for mask<N>.
template <size_t N>
inline constexpr auto mask_v = mask<N>::value;

/// \brief  Provides a way to query the maximum and minimum values that a
///   signed integer type of width \p N can hold.
///
/// \tparam N  The width in bits of the signed integer type to be queried.
template <size_t N>
struct limits {
  /// The type of a signed integer of at least \p N bits.
  using type = sinteger_t<N>;
  /// Returns the maximum finite value representable by a signed integer type of
  /// \p N bits.
  static constexpr type max () {
    return static_cast<type> ((uinteger_t<N>{1} << (N - 1U)) - 1);
  }
  /// Returns the minimum finite value representable by a signed integer type of
  /// \p N bits.
  static constexpr type min () { return static_cast<type> (-max () - 1); }
};

/// \brief Provides a way to query the maximum and minimum values that an
///   unsigned integer type of width \p N can hold.
///
/// \tparam N  The width in bits of the unsigned integer type to be queried.
template <size_t N>
struct ulimits {
  /// The type of an unsigned integer of at least \p N bits.
  using type = uinteger_t<N>;
  /// Returns the maximum finite value representable by an unsigned integer type
  /// of \p N bits.
  static constexpr type max () { return mask_v<N>; }
  /// Returns the minimum finite value representable by an unsigned integer type
  /// of \p N bits.
  static constexpr type min () { return 0U; }
};

/// \brief Holds a value of \p N bits which may be signed or unsigned.
///
/// \tparam N The number of bits that the type should hold.
/// \tparam IsUnsigned  True if the value is unsigned; false otherwise.
template <size_t N, bool IsUnsigned>
class nbit_scalar {
public:
  /// If \p IsUnsigned is true the unsigned integer type with at least \p N
  /// bits; the signed type otherwise.
  using type = std::conditional_t<IsUnsigned, uinteger_t<N>, sinteger_t<N>>;

  constexpr nbit_scalar () : x_{0} {}
  /// Casts from a standard integer type to nbit_scalar.
  explicit constexpr nbit_scalar (type const x) : x_{x} {}
  /// Copy constructor.
  constexpr nbit_scalar (nbit_scalar const&) = default;
  /// Assignment from the interlying standard integer type.
  nbit_scalar& operator= (type const x) {
    x_ = x;
    return *this;
  }
  /// Assignment.
  nbit_scalar& operator= (nbit_scalar const&) = default;

  /// Casts the stored value to a standard integer type.
  constexpr operator type () const { return x_; }

private:
  type x_ : N;
};

// *******************
// unsigned arithmetic
// *******************

// addu
// ~~~~
/// \name Unsigned Addition
/// Functions that perform saturating addition of unsigned integral
/// quantities from 4 to 64 bits.
/// @{

/// \brief Adds two unsigned values each \p N bits wide.
///
/// Returns \f$ 2^N-1 \f$ if the result cannot be represented in \p N bits.
///
/// \tparam N  The number of bits for the unsigned arguments and result. May
///   be in the range \f$ [4, 64] \f$.
/// \param x  The first of the two unsigned values to be added.
/// \param y  The second of the two unsigned values to be added.
/// \result  \p x + \p y or \f$ 2^N-1 \f$ if the result cannot be
///   represented in \p N bits.
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr uinteger_t<N> addu (uinteger_t<N> const x, uinteger_t<N> const y) {
  assert (x <= ulimits<N>::max () && "addu<> x value out of range");
  assert (y <= ulimits<N>::max () && "addu<> y value out of range");
  constexpr auto maxu = mask_v<N>;
  uinteger_t<N> res = x + y;
  res |= -((res < x) | (res > maxu));
  return res & maxu;
}
/// \brief Adds two unsigned 32 bit values.
///
/// Returns \f$ 2^{32}-1 \f$ if the result cannot be represented in 32 bits.
///
/// \param x  The first of the two unsigned 32 bit values to be added.
/// \param y  The second of the two unsigned 32 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{32}-1 \f$
///   (`std::numeric_limits<uint32_t>::max()`) if the result cannot be
///   represented in 32 bits.
constexpr uint32_t addu32 (uint32_t const x, uint32_t const y) {
  return addu<32> (x, y);
}
/// \brief Adds two unsigned 16 bit values.
///
/// Returns \f$ 2^{16}-1 \f$ if the result cannot be represented in 16 bits.
///
/// \param x The first of the two unsigned 16 bit values to be added.
/// \param y  The second of the two unsigned 16 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{16}-1 \f$
///   (`std::numeric_limits<uint16_t>::max()`) if the result cannot be
///   represented in 16 bits.
constexpr uint16_t addu16 (uint16_t const x, uint16_t const y) {
  return addu<16> (x, y);
}
/// \brief Adds two unsigned 8 bit values.
///
/// Returns \f$ 2^8-1 \f$ if the result cannot be represented in 8 bits.
///
/// \param x  The first of the two unsigned 8 bit values to be added.
/// \param y  The second of the two unsigned 8 bit values to be added.
/// \result  \p x + \p y or \f$ 2^8-1 \f$
///   (`std::numeric_limits<uint8_t>::max()`). if the result cannot be
///   represented in 8 bits.
constexpr uint8_t addu8 (uint8_t const x, uint8_t const y) {
  return addu<8> (x, y);
}
/// @}

// subu
// ~~~~
/// \name Unsigned Subtraction
/// Functions that perform saturating subtraction of unsigned integral
/// quantities from 4 to 64 bits.
/// @{

/// Computes the result of \p x - \p y. If the result overflows --- that is, the
/// result is too large to be representable with an unsigned integer of
/// \p N bits --- the returned value is \f$ 2^N-1 \f$.
///
/// \tparam N The number of bits for the unsigned arguments and result. May
///   be in the range \f$ [4, 64] \f$.
/// \param x  The value from which \p y is deducted.
/// \param y  The value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too large,
///   \f$ 2^N-1 \f$ (saturation::limits<N>::max()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr uinteger_t<N> subu (uinteger_t<N> const x, uinteger_t<N> const y) {
  assert (x <= ulimits<N>::max () && "subu<> x value out of range");
  assert (y <= ulimits<N>::max () && "subu<> y value out of range");
  constexpr auto maxu = mask_v<N>;
  uinteger_t<N> res = x - y;
  res &= -(res <= x);
  res &= maxu;
  return res;
}
/// \brief Computes the 32 bit unsigned result of \p x - \p y.
///
/// If the result overflows (that is, the correct result would be &ge;
/// \f$ 2^{32} \f$ ) the returned value is \f$ 2^{32}-1 \f$.
///
/// \param x  The 32 bit unsigned value from which \p y is deducted.
/// \param y  The 32 bit unsigned value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too large,
///   \f$ 2^{32}-1 \f$ (`std::numeric_limits<uint32_t>::max()`).
constexpr uint32_t subu32 (uint32_t const x, uint32_t const y) {
  return subu<32> (x, y);
}
/// \brief Computes the 16 bit unsigned result of \p x - \p y.
///
/// If the result overflows (that is, the correct result would be &ge;
/// \f$ 2^{16} \f$ ) the result is \f$ 2^{16}-1 \f$.
///
/// \param x  The 16 bit unsigned value from which \p y is deducted.
/// \param y  The 16 bit unsigned value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too large,
///   \f$ 2^{16}-1 \f$ (`std::numeric_limits<uint16_t>::max()`).
constexpr uint16_t subu16 (uint16_t const x, uint16_t const y) {
  return subu<16> (x, y);
}
/// \brief Computes the 8 bit unsigned result of \p x - \p y.
///
/// If the result overflows (that is, the correct result would be &ge;
/// \f$ 2^{8} \f$ ) the result is \f$ 2^{8}-1 \f$.
///
/// \param x  The 8 bit unsigned value from which \p y is deducted.
/// \param y  The 8 bit unsigned value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too large,
///   \f$ 2^{8}-1 \f$ (`std::numeric_limits<uint8_t>::max()`).
constexpr uint8_t subu8 (uint8_t const x, uint8_t const y) {
  return subu<8> (x, y);
}
/// @}

// divu
// ~~~~
/// \name Unsigned Division
/// Functions that perform saturating division of unsigned integral
/// quantities from 4 to 64 bits.
/// @{

/// \brief Computes the unsigned result of \p x / \p y.
///
/// \note Unsigned division cannot overflow; this function is provided for
///   symmetry and completeness.
///
/// \tparam N The number of bits for the arguments and result. May be any
///   in the range \f$ [4, 64] \f$.
/// \param x  The unsigned dividend.
/// \param y  The unsigned divisor.
/// \returns  \p x / \p y.
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr uinteger_t<N> divu (uinteger_t<N> const x, uinteger_t<N> const y) {
  assert (x <= ulimits<N>::max () && "divu<> x value out of range");
  assert (y <= ulimits<N>::max () && "divu<> y value out of range");
  return x / y;
}
/// \brief Computes the unsigned result of \p x / \p y.
///
/// \note Unsigned division cannot overflow; this function is provided for
///   symmetry and completeness.
///
/// \param x  The unsigned 32 bit dividend.
/// \param y  The unsigned 32 bit divisor.
/// \returns  \p x / \p y.
constexpr uint32_t divu32 (uint32_t const x, uint32_t const y) {
  return divu<32> (x, y);
}
/// \brief Computes the unsigned result of \p x / \p y.
///
/// \note Unsigned division cannot overflow; this function is provided for
///   symmetry and completeness.
///
/// \param x  The unsigned 16 bit dividend.
/// \param y  The unsigned 16 bit divisor.
/// \returns  \p x / \p y.
constexpr uint16_t divu16 (uint16_t const x, uint16_t const y) {
  return divu<16> (x, y);
}
/// \brief Computes the unsigned result of \p x / \p y.
///
/// \note Unsigned division cannot overflow; this function is provided for
///   symmetry and completeness.
///
/// \param x  The unsigned 8 bit dividend.
/// \param y  The unsigned 8 bit divisor.
/// \returns  \p x / \p y.
constexpr uint8_t divu8 (uint8_t const x, uint8_t const y) {
  return divu<8> (x, y);
}
/// @}

// mulu
// ~~~~
/// \name Unsigned Multiplication
/// Functions that perform saturating multiplication of unsigned
/// integral quantities from 4 to 32 bits.
/// @{

/// \brief Computes the value of \p x &times; \p y.
///
/// If the result overflows --- that is, the correct answer is too large to be
/// representable with an unsigned integer of \p N bits --- the returned value
/// is \f$ 2^N-1 \f$.
///
/// \tparam N The number of bits for the unsigned arguments and result. May
///   be in the range \f$ [4, 64] \f$.
/// \param x  The first value to be multiplied.
/// \param y  The second value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large,
///   \f$ 2^N-1 \f$ (saturation::limits<N>::max()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 32)>>
constexpr uinteger_t<N> mulu (uinteger_t<N> const x, uinteger_t<N> const y) {
  assert (x <= ulimits<N>::max () && "mulu<> x value out of range");
  assert (y <= ulimits<N>::max () && "mulu<> y value out of range");
  auto const res =
      static_cast<uinteger_t<N * 2U>> (x) * static_cast<uinteger_t<N * 2U>> (y);
  auto const hi = res >> N;
  auto const lo = static_cast<uinteger_t<N>> (res);
  return (lo | -!!hi) & mask_v<N>;
}
/// \brief Computes the unsigned 32 bit value of \p x &times; \p y.
///
/// If the result overflows --- that is, the correct answer is too large to be
/// representable by a 32 bit unsigned integer --- the returned value is
/// \f$ 2^{32}-1 \f$.
///
/// \param x  The first unsigned 32 bit value to be multiplied.
/// \param y  The second unsigned 32 bit value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large,
///   \f$ 2^{32}-1 \f$ (`std::numeric_limits<uint32_t>::max()`).
constexpr uint32_t mulu32 (uint32_t const x, uint32_t const y) {
  return mulu<32> (x, y);
}
/// \brief Computes the unsigned 16 bit value of \p x &times; \p y.
///
/// If the result overflows --- that is, the correct answer is too large to be
/// representable by a 16 bit unsigned integer --- the returned value is
/// \f$ 2^{16}-1 \f$.
///
/// \param x  The first unsigned 16 bit value to be multiplied.
/// \param y  The second unsigned 16 bit value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large,
///   \f$ 2^{16}-1 \f$ (`std::numeric_limits<uint16_t>::max()`).
constexpr uint16_t mulu16 (uint16_t const x, uint16_t const y) {
  return mulu<16> (x, y);
}
/// \brief Computes the unsigned 8 bit value of \p x &times; \p y.
///
/// If the result overflows --- that is, the correct answer is too large to be
/// representable by an 8 bit unsigned integer --- the returned value is
/// \f$ 2^{8}-1 \f$.
///
/// \param x  The first unsigned 8 bit value to be multiplied.
/// \param y  The second unsigned 8 bit value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large,
///   \f$ 2^{8}-1 \f$ (`std::numeric_limits<uint8_t>::max()`).
constexpr uint8_t mulu8 (uint8_t const x, uint8_t const y) {
  return mulu<8> (x, y);
}
/// @}

// *****************
// signed arithmetic
// *****************

/// \name Signed Addition
/// Functions that perform saturating addition of signed integral
/// quantities from 4 to 64 bits.
/// @{

// adds
// ~~~~
/// \brief Adds two signed values each \p N bits wide.
///
/// Returns \f$ 2^{N-1}-1 \f$ or \f$ -2^{N-1} \f$ if the result cannot be
/// represented in \p N bits.
///
/// \tparam N  The number of bits for the unsigned arguments and result. May
///   be in the range \f$ [4, 64] \f$.
/// \param x  The first of the two values to be added.
/// \param y  The second of the two values to be added.
/// \result  \p x + \p y or \f$ 2^{N-1}-1 \f$ if the result is positive but
///   cannot be represented in \p N bits; \f$ -2{N-1} \f$ if the result is
///   negative but cannot be represented in \p N bits.
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> adds (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= limits<N>::min () && x <= limits<N>::max () &&
          "adds<> x value out of range");
  assert (y >= limits<N>::min () && y <= limits<N>::max () &&
          "adds<> y value out of range");
  using uint = uinteger_t<N>;
  using sint = sinteger_t<N>;
  using ubits = nbit_scalar<N, true>;
  using sbits = nbit_scalar<N, false>;
  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  // Get the answer (with potential overflow).
  auto const res = ubits{static_cast<uint> (ux + uy)};

  // Calculate the overflowed result as max or min depending on the sign of ux.
  auto const v = sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ux >> (N - 1U)) + limits<N>::max ())})};
  assert (v == (x < sint{0} ? limits<N>::min () : limits<N>::max ()));

  // Check for overflow.
  if (sbits{static_cast<sint> ((static_cast<uint> (v) ^ uy) | ~(uy ^ res))} >=
      0) {
    return v;
  }
  // There was no overflow: return the proper result.
  return sbits{static_cast<sint> (res)};
}
/// \brief Adds two signed 32 bit values.
///
/// Returns \f$ 2^{31}-1 \f$ or \f$ -2^{31} \f$ if the result cannot be
/// represented in 32 bits.
///
/// \param x  The first of the two signed 32 bit values to be added.
/// \param y  The second of the two signed 32 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{31}-1 \f$ if the result is positive but
///   cannot be represented in 32 bits; \f$ -2{31} \f$ if the result is
///   negative but cannot be represented in 32 bits.
constexpr int32_t adds32 (int32_t const x, int32_t const y) {
  return adds<32> (x, y);
}
/// \brief Adds two signed 16 bit values.
///
/// Returns \f$ 2^{14}-1 \f$ or \f$ -2^{14} \f$ if the result cannot be
/// represented in 16 bits.
///
/// \param x  The first of the two signed 16 bit values to be added.
/// \param y  The second of the two signed 16 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{15}-1 \f$ if the result is positive but
///   cannot be represented in 16 bits; \f$ -2{15} \f$ if the result is
///   negative but cannot be represented in 16 bits.
constexpr int16_t adds16 (int16_t const x, int16_t const y) {
  return adds<16> (x, y);
}
/// \brief Adds two signed 8 bit values.
///
/// Returns \f$ 2^{7}-1 \f$ or \f$ -2^{7} \f$ if the result cannot be
/// represented in 8 bits.
///
/// \param x  The first of the two signed 8 bit values to be added.
/// \param y  The second of the two signed 8 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{7}-1 \f$ if the result is positive but
///   cannot be represented in 8 bits; \f$ -2{7} \f$ if the result is
///   negative but cannot be represented in 8 bits.
constexpr int16_t adds8 (int8_t const x, int8_t const y) {
  return adds<8> (x, y);
}
/// @}

// subs
// ~~~~
/// \name Signed Subtraction
/// Functions that perform saturating subtraction of signed integral
/// quantities from 4 to 64 bits.
/// @{

/// \brief Computes the signed result of \p x - \p y.
///
/// If the result overflows --- that is, the result is either too large or too
/// small to be representable with a twos-complement integer of \p N bits
/// --- the result is \f$ 2^{N-1}-1 \f$ or \f$ -2^{N-1} \f$ respectively.
///
/// \tparam N  The number of bits for the twos complement arguments and
///   result. May be in the range \f$ [4, 64] \f$.
/// \param x  The value from which \p y is deducted.
/// \param y  The value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too small, \f$ -2^{N-1} \f$
///   (saturation::limits<N>::min()); if the result would be too large,
///   \f$ 2^{N-1}-1 \f$ (saturation::limits<N>::max()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> subs (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= limits<N>::min () && x <= limits<N>::max () &&
          "subs<> x value out of range");
  assert (y >= limits<N>::min () && y <= limits<N>::max () &&
          "subs<> y value out of range");
  using uint = uinteger_t<N>;
  using sint = sinteger_t<N>;
  using ubits = nbit_scalar<N, true>;
  using sbits = nbit_scalar<N, false>;

  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  auto const res = ubits{static_cast<uint> (ux - uy)};

  // Check for overflow: ux must be different from uy and res.
  if (sbits{static_cast<sint> ((ux ^ uy) & (ux ^ res))} < 0) {
    // Calculate the overflowed result as max or min depending on the sign of
    // ux.
    auto const v = sbits{static_cast<sint> (
        ubits{static_cast<uint> ((ux >> (N - 1U)) + limits<N>::max ())})};
    assert (v == (x < sint{0} ? limits<N>::min () : limits<N>::max ()));
    return v;
  }
  // There was no overflow: return the proper result.
  return sbits{static_cast<sint> (res)};
}
/// \brief Computes the 32 bit signed result of \p x - \p y.
///
/// If the result overflows --- that is, the result is either too large or too
/// small to be representable by a twos-complement integer of 32 bits
/// --- the result is \f$ 2^{31}-1 \f$ or \f$ -2^{31} \f$ respectively.
///
/// \param x  The 32 bit signed value from which \p y is deducted.
/// \param y  The 32 bit signed value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too small, \f$ -2^{31} \f$
///   (std::numeric_limits<int32_t>::min()); if the result would be too large,
///   \f$ 2^{31}-1 \f$ (std::numeric_limits<int32_t>::max()).
constexpr int32_t subs32 (int32_t const x, int32_t const y) {
  return subs<32> (x, y);
}
/// \brief Computes the 16 bit signed result of \p x - \p y.
///
/// If the result overflows --- that is, the result is either too large or too
/// small to be representable by a twos-complement integer of 16 bits
/// --- the result is \f$ 2^{15}-1 \f$ or \f$ -2^{15} \f$ respectively.
///
/// \param x  The 16 bit signed value from which \p y is deducted.
/// \param y  The 16 bit signed value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too small, \f$ -2^{15} \f$
///   (std::numeric_limits<int16_t>::min()); if the result would be too large,
///   \f$ 2^{15}-1 \f$ (std::numeric_limits<int16_t>::max()).
constexpr int16_t subs16 (int16_t const x, int16_t const y) {
  return subs<16> (x, y);
}
/// \brief Computes the 8 bit signed result of \p x - \p y.
///
/// If the result overflows --- that is, the result is either too large or too
/// small to be representable by a twos-complement integer of 8 bits
/// --- the result is \f$ 2^7-1 \f$ or \f$ -2^7 \f$ respectively.
///
/// \param x  The 8 bit signed value from which \p y is deducted.
/// \param y  The 8 bit signed value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too small, \f$ -2^7 \f$
///   (std::numeric_limits<int8_t>::min()); if the result would be too large,
///   \f$ 2^7-1 \f$ (std::numeric_limits<int8_t>::max()).
constexpr int8_t subs8 (int8_t const x, int8_t const y) {
  return subs<8> (x, y);
}
/// @}

// divs
// ~~~~
/// \name Signed Division
/// Functions that perform saturating division of signed integral
/// quantities from 4 to 64 bits.
/// @{

/// \brief Computes the signed result of \p x / \p y.
///
/// \note Twos complement signed division can overflow because
///   \f$ \frac{-2^{N-1}}{-1} = 2^{N-1} \f$ and the largest value that can be
///   represented is \f$ 2^{N-1}-1 \f$.
///
/// \tparam N The number of bits for the twos complement arguments and
///   result. May be in the range \f$ [4, 64] \f$.
/// \param x  The signed dividend.
/// \param y  The signed divisor.
/// \returns  \p x / \p y. If the result would be too large and positive,
///   \f$ 2^{N-1}-1 \f$ (saturation::limits<N>::max()); if the result
///   would be too large and negative, \f$ -2^{N-1} \f$
///   (saturation::limits<N>::min()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> divs (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= limits<N>::min () && x <= limits<N>::max () &&
          "divs<> x value out of range");
  assert (y >= limits<N>::min () && y <= limits<N>::max () &&
          "divs<> y value out of range");
  using uint = uinteger_t<N>;
  using ubits = nbit_scalar<N, true>;
  return (x +
          !(ubits{static_cast<uint> (y + 1)} |
            ubits{static_cast<uint> (static_cast<uint> (x) +
                                     static_cast<uint> (limits<N>::min ()))})) /
         y;
}
/// \brief Computes the 32 bit signed result of \p x / \p y.
///
/// \note Twos complement signed division can overflow because
///   \f$ \frac{-2^{31}}{-1} = 2^{31} \f$.
///
/// \param x  The 32 bit signed dividend.
/// \param y  The 32 bit signed divisor.
/// \returns  \p x / \p y. If the result would be too large and positive,
///   \f$ 2^{31}-1 \f$ (that is, std::numeric_limits<int32_t>::max()); if the
///   result would be too large and negative, \f$ -2^{31} \f$
///   (that is, std::numeric_limits<int32_t>::min()).
constexpr int32_t divs32 (int32_t const x, int32_t const y) {
  return divs<32> (x, y);
}
/// \brief Computes the 16 bit signed result of \p x / \p y.
///
/// \note Twos complement signed division can overflow because
///   \f$ \frac{-2^{15}}{-1} = 2^{15} \f$.
///
/// \param x  The 16 bit signed dividend.
/// \param y  The 16 bit signed divisor.
/// \returns  \p x / \p y. If the result would be too large and positive,
///   \f$ 2^{15}-1 \f$ (that is, std::numeric_limits<int16_t>::max()); if the
///   result would be too large and negative, \f$ -2^{15} \f$
///   (that is, std::numeric_limits<int16_t>::min()).
constexpr int16_t divs16 (int16_t const x, int16_t const y) {
  return divs<16> (x, y);
}
/// \brief Computes the 8 bit signed result of \p x / \p y.
///
/// \note Twos complement signed division can overflow because
///   \f$ \frac{-2^7}{-1} = 2^7 \f$.
///
/// \param x  The 8 bit signed dividend.
/// \param y  The 8 bit signed divisor.
/// \returns  \p x / \p y. If the result would be too large and positive,
///   \f$ 2^7-1 \f$ (that is, std::numeric_limits<int8_t>::max()); if the
///   result would be too large and negative, \f$ -2^7 \f$
///   (that is, std::numeric_limits<int8_t>::min()).
constexpr int8_t divs8 (int8_t const x, int8_t const y) {
  return divs<8> (x, y);
}
/// @}

// muls
// ~~~~
/// \name Signed Multiplication
/// Functions that perform saturating multiplication of signed integral
/// quantities from 4 to 32 bits.
/// @{

/// \brief Computes the signed result of multiplying \p x by \p y.
///
/// \tparam N The number of bits for the twos complement arguments and
///   result. May be in the range \f$ [4, 32] \f$.
/// \param x  The first value to be multiplied.
/// \param y  The second value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large and positive,
///   \f$ 2^{N-1}-1 \f$ (saturation::limits<N>::max()); if the result
///   would be too large and negative, \f$ -2^{N-1} \f$
///   (saturation::limits<N>::min()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 32)>>
constexpr sinteger_t<N> muls (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= limits<N>::min () && x <= limits<N>::max () &&
          "muls<> x value out of range");
  assert (y >= limits<N>::min () && y <= limits<N>::max () &&
          "muls<> y value out of range");

  using s2bits = nbit_scalar<N * 2, false>;
  using s2int = typename s2bits::type;

  using sbits = nbit_scalar<N, false>;
  using sint = typename sbits::type;
  using ubits = nbit_scalar<N, true>;
  using uint = typename ubits::type;

  auto const res = s2bits{
      static_cast<s2int> (static_cast<s2int> (x) * static_cast<s2int> (y))};
  if (static_cast<sint> (res >> N) != static_cast<sint> (res >> (N - 1U))) {
    auto const v = sbits{static_cast<sint> (ubits{static_cast<uint> (
        static_cast<uint> (ubits{static_cast<uint> (x ^ y)} >> (N - 1U)) +
        static_cast<uint> (limits<N>::max ()))})};
    assert (v == (res < sint{0} ? limits<N>::min () : limits<N>::max ()));
    return v;
  }
  return static_cast<sint> (res);
}
/// \brief Computes the signed 32 bit result of multiplying \p x by \p y.
///
/// \param x  The first 32 bit signed value to be multiplied.
/// \param y  The second 32 bit signed value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large and positive,
///   \f$ 2^{31}-1 \f$ (`std::numeric_limits<int32_t>::max()`); if the result
///   would be too large and negative, \f$ -2^{31} \f$
///   (`std::numeric_limits<int32_t>::min()`).
constexpr int32_t muls32 (int32_t const x, int32_t const y) {
  return muls<32> (x, y);
}
/// \brief Computes the signed 16 bit result of multiplying \p x by \p y.
///
/// \param x  The first 16 bit signed value to be multiplied.
/// \param y  The second 16 bit signed value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large and positive,
///   \f$ 2^{15}-1 \f$ (`std::numeric_limits<int16_t>::max()`); if the result
///   would be too large and negative, \f$ -2^{15} \f$
///   (`std::numeric_limits<int16_t>::min()`).
constexpr int16_t muls16 (int16_t const x, int16_t const y) {
  return muls<16> (x, y);
}
/// \brief Computes the signed 8 bit result of multiplying \p x by \p y.
///
/// \param x  The first 8 bit signed value to be multiplied.
/// \param y  The second 8 bit signed value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large and positive,
///   \f$ 2^7-1 \f$ (`std::numeric_limits<int8_t>::max()`); if the result
///   would be too large and negative, \f$ -2^7 \f$
///   (`std::numeric_limits<int8_t>::min()`).
constexpr int8_t muls8 (int8_t const x, int8_t const y) {
  return muls<8> (x, y);
}
/// @}

}  // end namespace saturation

#endif  // SATURATION_HPP
