#include <gtest/gtest.h>

#include "../saturation.hpp"

using namespace saturation;

static_assert (mask<0>::value == 0);
static_assert (mask<1>::value == 0b1);
static_assert (mask<2>::value == 0b11);
static_assert (mask<3>::value == 0b111);
static_assert (mask<4>::value == 0xf);
static_assert (mask<7>::value == 0x7f);
static_assert (mask<8>::value == 0xff);
static_assert (mask<9>::value == 0x1ff);
static_assert (mask<15>::value == 0x7fff);
static_assert (mask<16>::value == 0xffff);
static_assert (mask<17>::value == 0x1ffff);
static_assert (mask<31>::value == 0x7fffffff);
static_assert (mask<32>::value == 0xffffffff);
static_assert (mask<33>::value == 0x1ffffffff);
static_assert (mask<63>::value == 0x7fffffffffffffff);
static_assert (mask<64>::value == std::numeric_limits<uint64_t>::max ());

static_assert (limits<8>::min () == std::numeric_limits<int8_t>::min ());
static_assert (limits<8>::max () == std::numeric_limits<int8_t>::max ());
static_assert (limits<16>::min () == std::numeric_limits<int16_t>::min ());
static_assert (limits<16>::max () == std::numeric_limits<int16_t>::max ());
static_assert (limits<32>::min () == std::numeric_limits<int32_t>::min ());
static_assert (limits<32>::max () == std::numeric_limits<int32_t>::max ());

static_assert (ulimits<8>::min () == std::numeric_limits<uint8_t>::min ());
static_assert (ulimits<8>::max () == std::numeric_limits<uint8_t>::max ());
static_assert (ulimits<16>::min () == std::numeric_limits<uint16_t>::min ());
static_assert (ulimits<16>::max () == std::numeric_limits<uint16_t>::max ());
static_assert (ulimits<32>::min () == std::numeric_limits<uint32_t>::min ());
static_assert (ulimits<32>::max () == std::numeric_limits<uint32_t>::max ());

class Saturation32 : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int32_t>::min ();
  static constexpr auto max = std::numeric_limits<int32_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint32_t>::max ();
};

TEST_F (Saturation32, SignedAdd) {
  EXPECT_EQ (adds32 (0, 0), 0);
  EXPECT_EQ (adds32 (3, 5), 8);
  EXPECT_EQ (adds32 (min, min), min);
  EXPECT_EQ (adds32 (max, max), max);
  EXPECT_EQ (adds32 (16777216, 2130706432), max);
}

TEST_F (Saturation32, SignedSubtract) {
  EXPECT_EQ (subs32 (0, 0), 0);
  EXPECT_EQ (subs32 (0, 1), -1);
  EXPECT_EQ (subs32 (min, 1), min);
  EXPECT_EQ (subs32 (0, min), max);
  EXPECT_EQ (subs32 (max, 0), max);
  EXPECT_EQ (subs32 (max, max), 0);
  EXPECT_EQ (subs32 (max, 1), max - 1);
}

TEST_F (Saturation32, SignedDivide) {
  EXPECT_EQ (divs32 (0, 1), 0);
  EXPECT_EQ (divs32 (4, 2), 2);
  EXPECT_EQ (divs32 (max, 2), max / 2);
  EXPECT_EQ (divs32 (min, -1), max);
}

TEST_F (Saturation32, SignedMultiply) {
  EXPECT_EQ (muls32 (0, 0), 0);
  EXPECT_EQ (muls32 (3, 5), 15);
  EXPECT_EQ (muls32 (-3, 5), -15);
  EXPECT_EQ (muls32 (3, -5), -15);
  EXPECT_EQ (muls32 (3, -1342177280), min);
  EXPECT_EQ (muls32 (3, 997556224), max);
}

TEST_F (Saturation32, UnsignedAdd) {
  EXPECT_EQ (addu32 (0, 0), 0U);
  EXPECT_EQ (addu32 (maxu, 1), maxu);
}

TEST_F (Saturation32, UnsignedSubtract) {
  EXPECT_EQ (subu32 (0, 0), 0U);
  EXPECT_EQ (subu32 (4, 1), 3U);
  EXPECT_EQ (subu32 (0, 1), 0U);
}

TEST_F (Saturation32, UnsignedDivide) {
  EXPECT_EQ (divu32 (0, 16843009), 0U);
}

TEST_F (Saturation32, UnsignedMultiply) {
  EXPECT_EQ (mulu32 (0, 0), 0U);
  EXPECT_EQ (mulu32 (3, 5), 15U);
  EXPECT_EQ (mulu32 (13862387, 1076719596), 4294967295U);
}

class Saturation16 : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int16_t>::min ();
  static constexpr auto max = std::numeric_limits<int16_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint16_t>::max ();
};

TEST_F (Saturation16, SignedAdd) {
  EXPECT_EQ (adds16 (0, 0), 0);
  EXPECT_EQ (adds16 (3, 5), 8);
  EXPECT_EQ (adds16 (min, min), min);
  EXPECT_EQ (adds16 (max, max), max);
  EXPECT_EQ (adds16 (16772, 21332), max);
}
TEST_F (Saturation16, SignedSubtract) {
  EXPECT_EQ (subs16 (0, 0), 0);
  EXPECT_EQ (subs16 (0, 1), -1);
  EXPECT_EQ (subs16 (min, 1), min);
  EXPECT_EQ (subs16 (0, min), max);
  EXPECT_EQ (subs16 (max, 0), max);
  EXPECT_EQ (subs16 (max, max), 0);
  EXPECT_EQ (subs16 (max, 1), max - 1);
}
TEST_F (Saturation16, SignedDivide) {
  EXPECT_EQ (divs16 (0, 1), 0);
  EXPECT_EQ (divs16 (4, 2), 2);
  EXPECT_EQ (divs16 (max, 2), max / 2);
  EXPECT_EQ (divs16 (min, -1), max);
}
TEST_F (Saturation16, SignedMultiply) {
  EXPECT_EQ (muls16 (0, 0), 0);
  EXPECT_EQ (muls16 (3, 5), 15);
  EXPECT_EQ (muls16 (-3, 5), -15);
  EXPECT_EQ (muls16 (int16_t{3}, int16_t{-5}), int16_t{-15});
  EXPECT_EQ (muls16 (max, int16_t{1}), max);
  EXPECT_EQ (muls16 (max, 2), max);
  EXPECT_EQ (muls16 (min, 1), min);
  EXPECT_EQ (muls16 (min, int16_t{-1}), max);
  EXPECT_EQ (muls16 (min, int16_t{-2}), max);
}
TEST_F (Saturation16, UnsignedAdd) {
  EXPECT_EQ (addu16 (0, 0), 0U);
  EXPECT_EQ (addu16 (maxu, 1), maxu);
}
TEST_F (Saturation16, UnsignedSubtract) {
  EXPECT_EQ (subu16 (0, 0), 0U);
  EXPECT_EQ (subu16 (0, 1), 0U);
}
TEST_F (Saturation16, UnsignedDivide) {
  EXPECT_EQ (divu16 (0, 1364), 0U);
}
TEST_F (Saturation16, UnsignedMultiply) {
  EXPECT_EQ (mulu16 (0, 0), 0U);
  EXPECT_EQ (mulu16 (3, 5), 15U);
  EXPECT_EQ (mulu16 (32768, 3), maxu);
}

class Saturation8 : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int8_t>::min ();
  static constexpr auto max = std::numeric_limits<int8_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint8_t>::max ();
};

TEST_F (Saturation8, SignedAdd) {
  EXPECT_EQ (adds8 (0, 0), 0);
  EXPECT_EQ (adds8 (3, 5), 8);
  EXPECT_EQ (adds8 (min, min), min);
  EXPECT_EQ (adds8 (max, max), max);
  EXPECT_EQ (adds8 (106, 97), max);
}
TEST_F (Saturation8, SignedSubtract) {
  EXPECT_EQ (subs8 (0, 0), 0);
  EXPECT_EQ (subs8 (0, 1), -1);
  EXPECT_EQ (subs8 (min, 1), min);
  EXPECT_EQ (subs8 (0, min), max);
  EXPECT_EQ (subs8 (max, 0), max);
  EXPECT_EQ (subs8 (max, max), 0);
  EXPECT_EQ (subs8 (max, 1), max - 1);
}
TEST_F (Saturation8, SignedDivide) {
  EXPECT_EQ (divs8 (0, 1), 0);
  EXPECT_EQ (divs8 (4, 2), 2);
  EXPECT_EQ (divs8 (max, 2), max / 2);
  EXPECT_EQ (divs8 (min, -1), max);
}
TEST_F (Saturation8, SignedMultiply) {
  EXPECT_EQ (muls8 (0, 0), 0);
  EXPECT_EQ (muls8 (3, 2), 6);
  EXPECT_EQ (muls8 (-3, 2), -6);
  EXPECT_EQ (muls8 (3, -2), -6);
  EXPECT_EQ (muls8 (max, 1), max);
  EXPECT_EQ (muls8 (max, 2), max);
  EXPECT_EQ (muls8 (min, 1), min);
  EXPECT_EQ (muls8 (min, -1), max);
  EXPECT_EQ (muls8 (min, -2), max);
}
TEST_F (Saturation8, UnsignedAdd) {
  EXPECT_EQ (addu8 (0, 0), 0U);
  EXPECT_EQ (addu8 (maxu, 1), maxu);
}
TEST_F (Saturation8, UnsignedSubtract) {
  EXPECT_EQ (subu8 (0, 0), 0U);
  EXPECT_EQ (subu8 (0, 1), 0U);
}
TEST_F (Saturation8, UnsignedDivide) {
  EXPECT_EQ (divu8 (0, 43), 0U);
}
TEST_F (Saturation8, UnsignedMultiply) {
  EXPECT_EQ (mulu8 (0, 0), 0U);
  EXPECT_EQ (mulu8 (3, 5), 15U);
  EXPECT_EQ (mulu8 (254, 3), maxu);
}

template <typename T>
class Saturation : public testing::Test {};
TYPED_TEST_SUITE_P (Saturation);

TYPED_TEST_P (Saturation, UnsignedAdd) {
  constexpr auto n = TypeParam::value;
  using uint_type = uinteger_t<n>;
  constexpr auto maxu = ulimits<n>::max ();
  EXPECT_EQ (addu<n> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (addu<n> (uint_type{2}, uint_type{3}), uint_type{5});
  EXPECT_EQ (addu<n> (maxu, uint_type{1}), maxu);
}

TYPED_TEST_P (Saturation, SignedAdd) {
  constexpr auto bits = TypeParam::value;
  using sint_type = sinteger_t<bits>;
  constexpr auto max = limits<bits>::max ();
  constexpr auto min = limits<bits>::min ();
  EXPECT_EQ (adds<bits> (sint_type{0}, sint_type{0}), sint_type{0});
  EXPECT_EQ (adds<bits> (sint_type{1}, sint_type{3}), sint_type{4});
  EXPECT_EQ (adds<bits> (min, min), min);
  EXPECT_EQ (adds<bits> (max, max), max);
}

TYPED_TEST_P (Saturation, UnsignedSubtract) {
  constexpr auto bits = TypeParam::value;
  using uint_type = uinteger_t<bits>;
  constexpr auto maxu = ulimits<bits>::max ();
  EXPECT_EQ (subu<bits> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (subu<bits> (uint_type{1}, uint_type{0}), uint_type{1});
  EXPECT_EQ (subu<bits> (uint_type{5}, uint_type{3}), uint_type{2});
  EXPECT_EQ (subu<bits> (uint_type{0}, uint_type{1}), uint_type{0});
  EXPECT_EQ (subu<bits> (maxu, uint_type{0}), maxu);
  EXPECT_EQ (subu<bits> (maxu, maxu), uint_type{0});
}
TYPED_TEST_P (Saturation, SignedSubtract) {
  constexpr auto bits = TypeParam::value;
  using sint_type = sinteger_t<bits>;
  constexpr auto max = limits<bits>::max ();
  constexpr auto min = limits<bits>::min ();
  EXPECT_EQ (subs<bits> (sint_type{0}, sint_type{0}), sint_type{0});
  EXPECT_EQ (subs<bits> (sint_type{0}, sint_type{1}), sint_type{-1});
  EXPECT_EQ (subs<bits> (min, sint_type{1}), min);
  EXPECT_EQ (subs<bits> (0, min), max);
  EXPECT_EQ (subs<bits> (max, sint_type{0}), max);
  EXPECT_EQ (subs<bits> (max, max), sint_type{0});
  EXPECT_EQ (subs<bits> (max, sint_type{1}), sint_type{max - 1});
}
TYPED_TEST_P (Saturation, UnsignedDivide) {
  constexpr auto bits = TypeParam::value;
  using uint_type = uinteger_t<bits>;
  constexpr auto maxu = ulimits<bits>::max ();
  EXPECT_EQ (divu<bits> (uint_type{0}, uint_type{3}), uint_type{0});
  EXPECT_EQ (divu<bits> (uint_type{10}, uint_type{2}), uint_type{5});
  EXPECT_EQ (divu<bits> (maxu, uint_type{1}), maxu);
}
TYPED_TEST_P (Saturation, SignedDivide) {
  constexpr auto bits = TypeParam::value;
  using sint_type = sinteger_t<bits>;
  constexpr auto max = limits<bits>::max ();
  constexpr auto min = limits<bits>::min ();
  EXPECT_EQ (divs<bits> (sint_type{0}, sint_type{1}), sint_type{0});
  EXPECT_EQ (divs<bits> (sint_type{4}, sint_type{2}), sint_type{2});
  EXPECT_EQ (divs<bits> (max, sint_type{2}), sint_type{max / 2});
  EXPECT_EQ (divs<bits> (min, sint_type{-1}), max);
}
TYPED_TEST_P (Saturation, UnsignedMultiply) {
  constexpr auto bits = TypeParam::value;
  if constexpr (bits <= 32U) {
    using uint_type = uinteger_t<bits>;
    constexpr auto maxu = mask_v<bits>;
    EXPECT_EQ (mulu<bits> (uint_type{0}, uint_type{0}), uint_type{0});
    EXPECT_EQ (mulu<bits> (uint_type{3}, uint_type{5}), uint_type{15});
    EXPECT_EQ (mulu<bits> (maxu, uint_type{1}), maxu);
    EXPECT_EQ (mulu<bits> (maxu >> 1U, uint_type{2}), uint_type{maxu - 1U});
    EXPECT_EQ (mulu<bits> (maxu, uint_type{2}), maxu);
    if constexpr (bits > 16U) {
      EXPECT_EQ (mulu<bits> (uint_type{13862387}, uint_type{1076719596}),
                 uint_type{4294967295U & maxu});
    }
  }
}
TYPED_TEST_P (Saturation, SignedMultiply) {
  constexpr auto bits = TypeParam::value;
  if constexpr (bits <= 32U) {
    using sint_type = sinteger_t<bits>;
    constexpr auto max = limits<bits>::max ();
    constexpr auto min = limits<bits>::min ();
    EXPECT_EQ (muls<bits> (sint_type{0}, sint_type{0}), sint_type{0});
    EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{2}), sint_type{6});
    EXPECT_EQ (muls<bits> (sint_type{-3}, sint_type{2}), sint_type{-6});
    EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{-2}), sint_type{-6});
    EXPECT_EQ (muls<bits> (max, sint_type{1}), max);
    EXPECT_EQ (muls<bits> (max, sint_type{2}), max);
    EXPECT_EQ (muls<bits> (min, sint_type{1}), min);
    EXPECT_EQ (muls<bits> (min, sint_type{-1}), max);
    EXPECT_EQ (muls<bits> (min, sint_type{-2}), max);
    EXPECT_EQ (muls<bits> (sint_type{3}, static_cast<sint_type> (min + 2)),
               min);
    EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{max - 2}), max);
  }
}

REGISTER_TYPED_TEST_SUITE_P (Saturation, UnsignedAdd, SignedAdd, SignedSubtract,
                             UnsignedSubtract, UnsignedDivide, SignedDivide,
                             UnsignedMultiply, SignedMultiply);
template <unsigned Value>
using unsigned_constant = std::integral_constant<unsigned, Value>;
using width_types = testing::Types<
    unsigned_constant<4U>, unsigned_constant<7U>, unsigned_constant<8U>,
    unsigned_constant<9U>, unsigned_constant<15U>, unsigned_constant<16U>,
    unsigned_constant<17U>, unsigned_constant<24U>, unsigned_constant<31U>,
    unsigned_constant<32U>, unsigned_constant<33U>, unsigned_constant<63U>,
    unsigned_constant<64U>>;
INSTANTIATE_TYPED_TEST_SUITE_P (ExplicitWidths, Saturation, width_types, );
