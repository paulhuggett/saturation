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
#ifndef NO_INLINE_ASM
#if defined(__GNUC__) && defined(__x86_64__)
namespace details {

/// An x86-only implementation of saturating unsigned subtract which is suitable
/// for register-sized values of \p N.
///
/// \tparam N  The number of bits for the unsigned arguments and result. Must be
///   plausibly the size of a target processor register.
/// \param x  The first of the two unsigned values to be subtracted.
/// \param y  The second of the two unsigned values to be subtracted.
/// \result  \p x - \p y or \f$ 2^N-1 \f$ if the result cannot be represented
///   in \p N bits.
template <size_t N, typename = typename std::enable_if_t<is_register_width (N)>>
inline uinteger_t<N> subu_asm (uinteger_t<N> x, uinteger_t<N> y) {
  uinteger_t<N> t = 0;
  __asm__(
      "sub {%[y],%[x] | %[x],%[y]}\n\t"  // x -= y (sets carry C on overflow)
      "cmovc {%[t],%[x] | %[x],%[t]}"    // if C, x = 0.
      : [x] "+&r"(x)                     // output
      : [y] "irm"(y), [t] "r"(t)         // input
      : "cc"                             // clobber
  );
  return x;
}

}  // end namespace details

template <>
inline uinteger_t<16> subu<16, std::enable_if_t<true>> (
    uinteger_t<16> const x, uinteger_t<16> const y) {
  return details::subu_asm<16> (x, y);
}
template <>
inline uinteger_t<32> subu<32, std::enable_if_t<true>> (
    uinteger_t<32> const x, uinteger_t<32> const y) {
  return details::subu_asm<32> (x, y);
}
template <>
inline uinteger_t<64> subu<64, std::enable_if_t<true>> (
    uinteger_t<64> const x, uinteger_t<64> const y) {
  return details::subu_asm<64> (x, y);
}
#endif  // __GNUC__ && __x86_64__
#endif  // NO_INLINE_ASM

/// \brief Computes the 32 bit unsigned result of \p x - \p y.
///
/// If the result overflows (that is, the correct result would be &ge;
/// \f$ 2^{32} \f$ ) the returned value is \f$ 2^{32}-1 \f$.
///
/// \param x  The 32 bit unsigned value from which \p y is deducted.
/// \param y  The 32 bit unsigned value deducted from \p x.
/// \returns  \p x - \p y. If the result would be too large,
///   \f$ 2^{32}-1 \f$ (`std::numeric_limits<uint32_t>::max()`).
inline uint32_t subu32 (uint32_t const x, uint32_t const y) {
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
inline uint16_t subu16 (uint16_t const x, uint16_t const y) {
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
inline uint8_t subu8 (uint8_t const x, uint8_t const y) {
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
