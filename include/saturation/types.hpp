#ifndef SATURATION_TYPES_HPP
#define SATURATION_TYPES_HPP

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

}  // end namespace saturation

#endif  // SATURATION_TYPES_HPP
