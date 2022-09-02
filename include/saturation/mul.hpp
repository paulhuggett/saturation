#ifndef SATURATION_MUL_HPP
#define SATURATION_MUL_HPP

#include <cassert>

#include "saturation/types.hpp"

namespace saturation {

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
      // The above 'else' is necessary to avoid a warning from VS2022 about
      // unreachable code (C4702).
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

#endif  // SATURATION_MUL_HPP