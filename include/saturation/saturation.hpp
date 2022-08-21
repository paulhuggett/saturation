/// \file saturation.hpp
/// \brief A collection of functions which provide saturating arithmetic
/// (addition, subtraction, multiplication, and division) for signed and
/// unsigned integer types.
///
/// Types can range from 4 to 64 bits. Operations using integers of width
/// matching target registers are likely to be branchless.

#ifndef SATURATION_HPP
#define SATURATION_HPP

#include <cassert>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>

#ifndef HAVE_INT128
#define HAVE_INT128 0
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
      static_cast<uinteger_t<N>> (uinteger_t<N>{mask<N - 1U>::value} << 1U) |
      uinteger_t<N>{1};
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
struct slimits {
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

namespace details {

/// Counts the number of set bits in a value. This version is sometimes
/// preferred to the compiler intrinsic because not all compilers define the
/// intrinsic as constexpr.
///
/// \tparam T  An unsigned integer type.
/// \param x  A value whose population count is to be returned.
/// \return  The population count of \p x.
template <typename T,
          typename = typename std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned pop_count (T const x) noexcept {
  return x == 0U ? 0U : (x & 1U) + pop_count (x >> 1U);
}

/// Returns true if \p n plausibly matches the number of bits in a target
/// machine register.
constexpr bool is_register_width (size_t const n) {
  return n % 8 == 0 && pop_count (n) == 1U;
}

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
  constexpr nbit_scalar (nbit_scalar const&) = default;
  constexpr nbit_scalar (nbit_scalar&&) noexcept = default;

  ~nbit_scalar () noexcept = default;

  /// Assignment from the interlying standard integer type.
  nbit_scalar& operator= (type const x) {
    x_ = x;
    return *this;
  }
  nbit_scalar& operator= (nbit_scalar const&) = default;
  nbit_scalar& operator= (nbit_scalar&&) noexcept = default;

  /// Casts the stored value to a standard integer type.
  constexpr operator type () const { return x_; }

private:
  type x_ : N;
};

}  // end namespace details

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
  assert (x <= ulimits<N>::max ());  // addu<> x value out of range
  assert (y <= ulimits<N>::max ());  // addu<> y value out of range
  constexpr auto maxu = mask_v<N>;
  uinteger_t<N> res = x + y;
  res |= -((res < x) | (res > maxu));
  return res & maxu;
}

#ifndef NO_INLINE_ASM
#if defined(__GNUC__) && defined(__x86_64__)
namespace details {

/// An x86-only implementation of saturating unsigned add which is suitable for
/// register-sized values of \p N.
///
/// \tparam N  The number of bits for the unsigned arguments and result. Must be
///   plausibly the size of a target processor register.
/// \param x  The first of the two unsigned values to be added.
/// \param y  The second of the two unsigned values to be added.
/// \result  \p x + \p y or \f$ 2^N-1 \f$ if the result cannot be represented
///   in \p N bits.
template <size_t N, typename = typename std::enable_if_t<is_register_width (N)>>
inline uinteger_t<N> addu_asm (uinteger_t<N> x, uinteger_t<N> y) {
  uinteger_t<N> t;
  __asm__(
      "add %[y], %[x]\n\t"        // x += y (sets carry C on overflow)
      "sbb %[t], %[t]\n\t"        // t -= t - C (t will become 0 or ~0).
      "or %[t], %[x]\n\t"         // x |= t
      : [x] "+r"(x), [t] "=r"(t)  // output
      : [y] "r"(y)                // input
  );
  return x;
}

}  // end namespace details

template <>
inline uinteger_t<8> addu<8, std::enable_if_t<true>> (uinteger_t<8> const x,
                                                      uinteger_t<8> const y) {
  return details::addu_asm<8> (x, y);
}
template <>
inline uinteger_t<16> addu<16, std::enable_if_t<true>> (
    uinteger_t<16> const x, uinteger_t<16> const y) {
  return details::addu_asm<16> (x, y);
}
template <>
inline uinteger_t<32> addu<32, std::enable_if_t<true>> (
    uinteger_t<32> const x, uinteger_t<32> const y) {
  return details::addu_asm<32> (x, y);
}
template <>
inline uinteger_t<64> addu<64, std::enable_if_t<true>> (
    uinteger_t<64> const x, uinteger_t<64> const y) {
  return details::addu_asm<64> (x, y);
}
#endif  // __GNUC__ && __x86_64__
#endif  // NO_INLINE_ASM

/// \brief Adds two unsigned 32 bit values.
///
/// Returns \f$ 2^{32}-1 \f$ if the result cannot be represented in 32 bits.
///
/// \param x  The first of the two unsigned 32 bit values to be added.
/// \param y  The second of the two unsigned 32 bit values to be added.
/// \result  \p x + \p y or \f$ 2^{32}-1 \f$
///   (`std::numeric_limits<uint32_t>::max()`) if the result cannot be
///   represented in 32 bits.
inline uint32_t addu32 (uint32_t const x, uint32_t const y) {
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
inline uint16_t addu16 (uint16_t const x, uint16_t const y) {
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
inline uint8_t addu8 (uint8_t const x, uint8_t const y) {
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
  assert (x <= ulimits<N>::max ());  // subu<> x value out of range
  assert (y <= ulimits<N>::max ());  // subu<> y value out of range
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
  assert (x <= ulimits<N>::max ());  // divu<> x value out of range
  assert (y <= ulimits<N>::max ());  // divu<> y value out of range
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

namespace details {

/// Multiplies two values in cases where the result type is too large for the
/// target.
///
/// \tparam T  An integral type.
/// \param u  The first of the two values to be multiplied.
/// \param v  The second of the two values to be multiplied.
/// \return  A pair consisting of the high and low-order parts respectively
///   of the result of multiplying \p u and \p v.
template <typename T,
          typename = typename std::enable_if_t<std::is_integral_v<T>>>
constexpr std::pair<T, T> multiply (T const u, T const v) {
  using UT = std::make_unsigned_t<T>;
  constexpr auto half_shift =
      sizeof (T) * CHAR_BIT / 2U;  // half the number of bits in type T.
  constexpr auto half_mask = (T{1} << half_shift) - 1U;

  auto const u_hi = u >> half_shift;
  auto const u_lo = static_cast<UT> (u) & half_mask;
  auto const v_hi = v >> half_shift;
  auto const v_lo = static_cast<UT> (v) & half_mask;

  auto const lo_lo = u_lo * v_lo;
  auto const lo_lo_mod = lo_lo & half_mask;
  auto const lo_lo_over = lo_lo >> half_shift;

  auto const hi_lo = static_cast<UT> (u_hi) * v_lo + lo_lo_over;
  auto const hi_lo_mod = hi_lo & half_mask;
  auto const hi_lo_over =
      static_cast<UT> (static_cast<T> (hi_lo) >> half_shift);

  auto const lo_hi = u_lo * static_cast<UT> (v_hi) + hi_lo_mod;
  auto const lo_hi_over =
      static_cast<UT> (static_cast<T> (lo_hi) >> half_shift);

  auto const hi =
      static_cast<T> (static_cast<UT> (u_hi * v_hi) + hi_lo_over + lo_hi_over);
  auto const lo = static_cast<T> ((lo_hi << half_shift) +
                                  lo_lo_mod);  // (Expression could be "u * v")
  return std::make_pair (hi, lo);
}

/// A function object which multiplies two values returning the result as pair
/// holding the high- and low-order parts respectively.
///
/// \tparam N The number of bits that the type should hold.
/// \tparam IsUnsigned  True if the value is unsigned; false otherwise.
template <size_t N, bool IsUnsigned>
struct multiplier {
  /// The type being multiplied.
  using arg_type = std::conditional_t<IsUnsigned, uinteger_t<N>, sinteger_t<N>>;

  /// Multiplies the arguments \p x and \p y returning a pair of values
  /// containing the high- and low-order parts of the result respectively.
  ///
  /// \param x  The first of the two values to be multiplied.
  /// \param y  The second of the two values to be multiplied.
  /// \return  A pair consisting of the high and low-order parts respectively
  ///   of the result of multiplying \p x and \p y.
  constexpr std::pair<arg_type, arg_type> operator() (arg_type const x,
                                                      arg_type const y) const {
    if constexpr (N <= narrow_multiply_max) {
      auto const narrow_res =
          static_cast<std::conditional_t<IsUnsigned, uinteger_t<N * 2>,
                                         sinteger_t<N * 2>>> (x) *
          y;
      return std::make_pair (static_cast<arg_type> (narrow_res >> N),
                             static_cast<arg_type> (narrow_res));
    } else {
      return adjust (multiply (x, y));
    }
  }

private:
  /// The largest value for N which the comiler can natively perform
  /// multiplication which having to resort to using the multiply()
  /// function.
  static constexpr auto narrow_multiply_max = size_t{32};
  /// The number of bits in arg_type.
  static constexpr auto shift = sizeof (arg_type) * CHAR_BIT - N;
  using unsigned_type = std::make_unsigned_t<arg_type>;

  /// multiply() returns a pair which uses all of the bits of both members to
  /// express the result. This function adjusts those values so that they
  /// consume only N bits each.
  static constexpr std::pair<arg_type, arg_type> adjust (
      std::pair<arg_type, arg_type>&& wide_res) {
    if constexpr (shift == 0) {
      return std::move (wide_res);
    } else {
      wide_res.first = static_cast<arg_type> (
          static_cast<unsigned_type> (wide_res.first) << shift |
          (static_cast<unsigned_type> (wide_res.second) >> N));
      wide_res.second =
          static_cast<arg_type> (static_cast<unsigned_type> (wide_res.second)
                                 << shift) >>
          shift;
      assert ((wide_res.first >> N) == 0 ||
              (!IsUnsigned &&
               (wide_res.first >> N) ==
                   static_cast<arg_type> (
                       -1)));  // multiplier<>-wide low result is out of range
      assert ((wide_res.second >> N) == 0 ||
              (!IsUnsigned &&
               (wide_res.second >> N) ==
                   static_cast<arg_type> (
                       -1)));  // multiplier<>-wide high result is out of range
      return std::move (wide_res);
    }
  }
};

}  // end namespace details

// mulu
// ~~~~
/// \name Unsigned Multiplication
/// Functions that perform saturating multiplication of unsigned
/// integral quantities from 4 to 64 bits.
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
///   \f$ 2^N-1 \f$ (saturation::slimits<N>::max()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr uinteger_t<N> mulu (uinteger_t<N> const x, uinteger_t<N> const y) {
  assert (x <= ulimits<N>::max ());  // mulu<> x value out of range
  assert (y <= ulimits<N>::max ());  // mulu<> y value out of range
  auto const [hi, lo] = details::multiplier<N, true>{}(x, y);
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
  assert (x >= slimits<N>::min () &&
          x <= slimits<N>::max ());  // adds<> x value out of range
  assert (y >= slimits<N>::min () &&
          y <= slimits<N>::max ());  // adds<> y value out of range
  using uint = uinteger_t<N>;
  using sint = sinteger_t<N>;
  using ubits = details::nbit_scalar<N, true>;
  using sbits = details::nbit_scalar<N, false>;
  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  // Get the answer (with potential overflow).
  auto const res = ubits{static_cast<uint> (ux + uy)};

  // Calculate the overflowed result as max or min depending on the sign of ux.
  auto const v = sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ux >> (N - 1U)) + slimits<N>::max ())})};
  assert (v == (x < sint{0} ? slimits<N>::min () : slimits<N>::max ()));

  // Check for overflow.
  if (sbits{static_cast<sint> ((static_cast<uint> (v) ^ uy) | ~(uy ^ res))} >=
      0) {
    return v;
  }
  // There was no overflow: return the proper result.
  return sbits{static_cast<sint> (res)};
}

#ifndef NO_INLINE_ASM
#if defined(__GNUC__) && defined(__x86_64__)
namespace details {

/// An x86-only implementation of saturating signed add which is suitable for
/// register-sized values of \p N.
///
/// \tparam N  The number of bits for the unsigned arguments and result. May
///   be in the range \f$ [4, 64] \f$.
/// \param x  The first of the two values to be added.
/// \param y  The second of the two values to be added.
/// \result  \p x + \p y or \f$ 2^{N-1}-1 \f$ if the result is positive but
///   cannot be represented in \p N bits; \f$ -2{N-1} \f$ if the result is
///   negative but cannot be represented in \p N bits.
template <size_t N, typename = typename std::enable_if_t<is_register_width (N)>>
inline sinteger_t<N> adds_asm (sinteger_t<N> x, sinteger_t<N> y) {
  static constexpr auto max = slimits<N>::max ();
  auto t = x;
  __asm__(
      "shr %[shift], %[t]\n\t"
      "add %[max], %[t]\n\t"
      "add %[y], %[x]\n\t"
      "cmovo %[t], %[x]\n\t"
      : [x] "+r"(x), [t] "+r"(t)                         // output
      : [y] "r"(y), [max] "r"(max), [shift] "i"(N - 1U)  // input
  );
  return x;
}

}  // end namespace details

template <>
inline sinteger_t<16> adds<16, std::enable_if_t<true>> (
    sinteger_t<16> const x, sinteger_t<16> const y) {
  return details::adds_asm<16> (x, y);
}
template <>
inline sinteger_t<32> adds<32, std::enable_if_t<true>> (
    sinteger_t<32> const x, sinteger_t<32> const y) {
  return details::adds_asm<32> (x, y);
}
template <>
inline sinteger_t<64> adds<64, std::enable_if_t<true>> (
    sinteger_t<64> const x, sinteger_t<64> const y) {
  return details::adds_asm<64> (x, y);
}
#endif  // __GNUC__ && __x86_64__
#endif  // NO_INLINE_ASM

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
inline int32_t adds32 (int32_t const x, int32_t const y) {
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
inline int16_t adds16 (int16_t const x, int16_t const y) {
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
///   (saturation::slimits<N>::min()); if the result would be too large,
///   \f$ 2^{N-1}-1 \f$ (saturation::slimits<N>::max()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> subs (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= slimits<N>::min () &&
          x <= slimits<N>::max ());  // subs<> x value out of range
  assert (y >= slimits<N>::min () &&
          y <= slimits<N>::max ());  // subs<> y value out of range
  using uint = uinteger_t<N>;
  using sint = sinteger_t<N>;
  using ubits = details::nbit_scalar<N, true>;
  using sbits = details::nbit_scalar<N, false>;

  // unsigned versions of x and y.
  auto const ux = ubits{static_cast<uint> (x)};
  auto const uy = ubits{static_cast<uint> (y)};
  auto const res = ubits{static_cast<uint> (ux - uy)};

  // Check for overflow: ux must be different from uy and res.
  if (sbits{static_cast<sint> (static_cast<uint> (ux ^ uy) &
                               static_cast<uint> (ux ^ res))} < 0) {
    // Calculate the overflowed result as max or min depending on the sign of
    // ux.
    auto const v = sbits{static_cast<sint> (
        ubits{static_cast<uint> ((ux >> (N - 1U)) + slimits<N>::max ())})};
    assert (v == (x < sint{0} ? slimits<N>::min () : slimits<N>::max ()));
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
///   \f$ 2^{N-1}-1 \f$ (saturation::slimits<N>::max()); if the result
///   would be too large and negative, \f$ -2^{N-1} \f$
///   (saturation::slimits<N>::min()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> divs (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= slimits<N>::min () &&
          x <= slimits<N>::max ());  // divs<> x value out of range
  assert (y >= slimits<N>::min () &&
          y <= slimits<N>::max ());  // divs<> y value out of range
  using uint = uinteger_t<N>;
  using ubits = details::nbit_scalar<N, true>;
  return (x + !(ubits{static_cast<uint> (y + 1)} |
                ubits{static_cast<uint> (
                    static_cast<uint> (x) +
                    static_cast<uint> (slimits<N>::min ()))})) /
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
/// quantities from 4 to 64 bits.
/// @{

/// \brief Computes the signed result of multiplying \p x by \p y.
///
/// \tparam N The number of bits for the twos complement arguments and
///   result. May be in the range \f$ [4, 32] \f$.
/// \param x  The first value to be multiplied.
/// \param y  The second value to be multiplied.
/// \returns  \p x &times; \p y. If the result would be too large and positive,
///   \f$ 2^{N-1}-1 \f$ (saturation::slimits<N>::max()); if the result
///   would be too large and negative, \f$ -2^{N-1} \f$
///   (saturation::slimits<N>::min()).
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> muls (sinteger_t<N> const x, sinteger_t<N> const y) {
  assert (x >= slimits<N>::min () &&
          x <= slimits<N>::max ());  // muls<> x value out of range
  assert (y >= slimits<N>::min () &&
          y <= slimits<N>::max ());  // muls<> y value out of range

  using sbits = details::nbit_scalar<N, false>;
  using sint = typename sbits::type;

  auto const [hi, lo] = details::multiplier<N, false>{}(x, y);
  if (hi != lo >> (N - 1)) {
    using ubits = details::nbit_scalar<N, true>;
    using uint = typename ubits::type;

    auto const v = sbits{static_cast<sint> (ubits{static_cast<uint> (
        static_cast<uint> (ubits{static_cast<uint> (static_cast<uint> (x) ^
                                                    static_cast<uint> (y))} >>
                           (N - 1U)) +
        static_cast<uint> (slimits<N>::max ()))})};
    assert (v == (hi < 0 ? slimits<N>::min () : slimits<N>::max ()));
    return v;
  }
  return static_cast<sint> (lo);
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
