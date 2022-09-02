#ifndef SATURATION_DIV_HPP
#define SATURATION_DIV_HPP

#include <cassert>

#include "saturation/types.hpp"

namespace saturation {

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

}  // end namespace saturation

#endif  // SATURATION_DIV_HPP
