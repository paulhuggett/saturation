#ifndef SATURATION_ADD_HPP
#define SATURATION_ADD_HPP

#include <cassert>

#include "saturation/types.hpp"

namespace saturation {

/// \name Unsigned Addition
/// Functions that perform saturating addition of unsigned integral
/// quantities from 4 to 64 bits.
/// @{

// addu
// ~~~~
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
      "sbb %[t], %[t]"            // t -= t - C (t will become 0 or ~0).
      : [x] "+r"(x), [t] "=r"(t)  // output
      : [y] "r"(y)                // input
      : "cc"                      // clobber
  );
  return x | t;
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

/// \name Signed Addition
/// Functions that perform saturating addition of signed integral
/// quantities from 4 to 64 bits.
/// @{

namespace details {

/// Calculate the overflowed result as max or min depending on the sign of x.
template <size_t N, typename = typename std::enable_if_t<(N >= 4 && N <= 64)>>
constexpr sinteger_t<N> adds_overflow_value (sinteger_t<N> const x) {
  using uint = uinteger_t<N>;
  using sint = sinteger_t<N>;
  using ubits = details::nbit_scalar<N, true>;
  using sbits = details::nbit_scalar<N, false>;
  return sbits{static_cast<sint> (
      ubits{static_cast<uint> ((ubits{static_cast<uint> (x)} >> (N - 1U)) + static_cast<uint>(slimits<N>::max ()))})};
}

} // end namespace details

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
  auto const v = details::adds_overflow_value<N> (x);
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
  sinteger_t<N> const v = details::adds_overflow_value<N> (x);
  __asm__(
      "add   %[y], %[x]\n\t"    // x += y (sets C on overflow)
      "cmovo %[v], %[x]"        // if C, x = v
      : [x] "+&r"(x)            // output
      : [y] "r"(y), [v] "r"(v)  // input
      : "cc"                    // clobber
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

}  // end namespace saturation

#endif  // SATURATION_ADD_HPP
