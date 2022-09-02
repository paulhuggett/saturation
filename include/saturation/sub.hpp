#ifndef SATURATION_SUB_HPP
#define SATURATION_SUB_HPP

#include <cassert>

#include "saturation/types.hpp"

namespace saturation {

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

}  // end namespace saturation

#endif  // SATURATION_SUB_HPP
