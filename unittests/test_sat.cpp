#include <gtest/gtest.h>

#include "saturation/saturation.hpp"

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

static_assert (slimits<8>::min () == std::numeric_limits<int8_t>::min ());
static_assert (slimits<8>::max () == std::numeric_limits<int8_t>::max ());
static_assert (slimits<16>::min () == std::numeric_limits<int16_t>::min ());
static_assert (slimits<16>::max () == std::numeric_limits<int16_t>::max ());
static_assert (slimits<32>::min () == std::numeric_limits<int32_t>::min ());
static_assert (slimits<32>::max () == std::numeric_limits<int32_t>::max ());

static_assert (ulimits<8>::min () == std::numeric_limits<uint8_t>::min ());
static_assert (ulimits<8>::max () == std::numeric_limits<uint8_t>::max ());
static_assert (ulimits<16>::min () == std::numeric_limits<uint16_t>::min ());
static_assert (ulimits<16>::max () == std::numeric_limits<uint16_t>::max ());
static_assert (ulimits<32>::min () == std::numeric_limits<uint32_t>::min ());
static_assert (ulimits<32>::max () == std::numeric_limits<uint32_t>::max ());

TEST (Mulu, 48) {
  using uint_type = uinteger_t<48>;
  EXPECT_EQ (mulu<48> (uint_type{0x800000000000}, uint_type{0x000100000000}),
             uint_type{0xFFFFFFFFFFFF});
  EXPECT_EQ (mulu<48> (uint_type{140737488355328}, uint_type{1099511627776}),
             uint_type{281474976710655});
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
  EXPECT_EQ (addu<n> (uint_type{0}, maxu), maxu);
}
TYPED_TEST_P (Saturation, SignedAdd) {
  constexpr auto bits = TypeParam::value;
  using sint_type = sinteger_t<bits>;
  constexpr auto max = slimits<bits>::max ();
  constexpr auto min = slimits<bits>::min ();
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
  constexpr auto max = slimits<bits>::max ();
  constexpr auto min = slimits<bits>::min ();
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
  constexpr auto max = slimits<bits>::max ();
  constexpr auto min = slimits<bits>::min ();
  EXPECT_EQ (divs<bits> (sint_type{0}, sint_type{1}), sint_type{0});
  EXPECT_EQ (divs<bits> (sint_type{4}, sint_type{2}), sint_type{2});
  EXPECT_EQ (divs<bits> (max, sint_type{2}), sint_type{max / 2});
  EXPECT_EQ (divs<bits> (min, sint_type{-1}), max);
}
TYPED_TEST_P (Saturation, UnsignedMultiply) {
  constexpr auto bits = TypeParam::value;
  using uint_type = uinteger_t<bits>;
  constexpr auto maxu = mask_v<bits>;
  EXPECT_EQ (mulu<bits> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (mulu<bits> (uint_type{3}, uint_type{5}), uint_type{15});
  EXPECT_EQ (mulu<bits> (maxu, uint_type{1}), maxu);
  EXPECT_EQ (mulu<bits> (maxu >> 1U, uint_type{2}), uint_type{maxu - 1U});
  EXPECT_EQ (mulu<bits> (maxu, uint_type{2}), maxu);
  EXPECT_EQ (mulu<bits> (uint_type{2}, uint_type{maxu - 2}), maxu);
}
TYPED_TEST_P (Saturation, SignedMultiply) {
  constexpr auto bits = TypeParam::value;
  using sint_type = sinteger_t<bits>;
  constexpr auto max = slimits<bits>::max ();
  constexpr auto min = slimits<bits>::min ();
  EXPECT_EQ (muls<bits> (sint_type{0}, sint_type{0}), sint_type{0});
  EXPECT_EQ (muls<bits> (sint_type{-1}, sint_type{-1}), sint_type{1});
  EXPECT_EQ (muls<bits> (sint_type{1}, sint_type{-1}), sint_type{-1});
  EXPECT_EQ (muls<bits> (sint_type{-1}, sint_type{1}), sint_type{-1});
  EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{2}), sint_type{6});
  EXPECT_EQ (muls<bits> (sint_type{-3}, sint_type{2}), sint_type{-6});
  EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{-2}), sint_type{-6});
  EXPECT_EQ (muls<bits> (max, sint_type{1}), max);
  EXPECT_EQ (muls<bits> (max, sint_type{2}), max);
  EXPECT_EQ (muls<bits> (max, sint_type{-1}), static_cast<sint_type> (min + 1));
  EXPECT_EQ (muls<bits> (max, sint_type{-2}), min);
  EXPECT_EQ (muls<bits> (min, sint_type{1}), min);
  EXPECT_EQ (muls<bits> (min, sint_type{-1}), max);
  EXPECT_EQ (muls<bits> (min, sint_type{-2}), max);
  EXPECT_EQ (muls<bits> (sint_type{3}, static_cast<sint_type> (min + 2)), min);
  EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{max - 2}), max);
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
    unsigned_constant<32U>, unsigned_constant<33U>, unsigned_constant<48U>,
    unsigned_constant<63U>, unsigned_constant<64U>>;
INSTANTIATE_TYPED_TEST_SUITE_P (ExplicitWidths, Saturation, width_types, );
