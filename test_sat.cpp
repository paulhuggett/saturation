#include <gtest/gtest.h>

#include "saturation.hpp"

static_assert (sat::mask<0>::value == 0);
static_assert (sat::mask<1>::value == 0b1);
static_assert (sat::mask<2>::value == 0b11);
static_assert (sat::mask<3>::value == 0b111);
static_assert (sat::mask<4>::value == 0xf);
static_assert (sat::mask<7>::value == 0x7f);
static_assert (sat::mask<8>::value == 0xff);
static_assert (sat::mask<9>::value == 0x1ff);
static_assert (sat::mask<15>::value == 0x7fff);
static_assert (sat::mask<16>::value == 0xffff);
static_assert (sat::mask<17>::value == 0x1ffff);
static_assert (sat::mask<31>::value == 0x7fffffff);
static_assert (sat::mask<32>::value == 0xffffffff);
static_assert (sat::mask<33>::value == 0x1ffffffff);
static_assert (sat::mask<63>::value == 0x7fffffffffffffff);
static_assert (sat::mask<64>::value == std::numeric_limits<uint64_t>::max());


class Saturation32 : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int32_t>::min ();
  static constexpr auto max = std::numeric_limits<int32_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint32_t>::max ();
};

TEST_F (Saturation32, Adds32) {
  EXPECT_EQ (sat::adds32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::adds32 (int32_t{3}, int32_t{5}), int32_t{8});
  EXPECT_EQ (sat::adds32 (min, min), min);
  EXPECT_EQ (sat::adds32 (max, max), max);
  EXPECT_EQ (sat::adds32 (int32_t{16777216}, int32_t{2130706432}), max);
}

TEST_F (Saturation32, Subs32) {
  EXPECT_EQ (sat::subs32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::subs32 (int32_t{0}, int32_t{1}), int32_t{-1});
  EXPECT_EQ (sat::subs32 (min, int32_t{1}), min);
  EXPECT_EQ (sat::subs32 (0, min), max);
  EXPECT_EQ (sat::subs32 (max, int32_t{0}), max);
  EXPECT_EQ (sat::subs32 (max, max), int32_t{0});
  EXPECT_EQ (sat::subs32 (max, int32_t{1}), int32_t{max - 1});
}

TEST_F (Saturation32, Divs32) {
  EXPECT_EQ (sat::divs32 (int32_t{0}, int32_t{1}), int32_t{0});
  EXPECT_EQ (sat::divs32 (int32_t{4}, int32_t{2}), int32_t{2});
  EXPECT_EQ (sat::divs32 (max, int32_t{2}), int32_t{max / 2});
  EXPECT_EQ (sat::divs32 (min, int32_t{-1}), max);
}

TEST_F (Saturation32, Muls32) {
  EXPECT_EQ (sat::muls32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{5}), int32_t{15});
  EXPECT_EQ (sat::muls32 (int32_t{-3}, int32_t{5}), int32_t{-15});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{-5}), int32_t{-15});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{-1342177280}), min);
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{997556224}), max);
}

TEST_F (Saturation32, Addu32) {
  EXPECT_EQ (sat::addu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::addu32 (maxu, uint32_t{1}), maxu);
}

TEST_F (Saturation32, Subu32) {
  EXPECT_EQ (sat::subu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::subu32 (uint32_t{0}, uint32_t{1}), uint32_t{0});
}

TEST_F (Saturation32, Divu32) {
  EXPECT_EQ (sat::divu32 (uint32_t{0}, uint32_t{16843009}), uint32_t{0});
}

TEST_F (Saturation32, Mulu32) {
  EXPECT_EQ (sat::mulu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::mulu32 (uint32_t{3}, uint32_t{5}), uint32_t{15});
  EXPECT_EQ (sat::mulu32 (uint32_t{13862387}, uint32_t{1076719596}),
             uint32_t{4294967295});
}

template <typename T>
class Saturation : public testing::Test {};
TYPED_TEST_SUITE_P (Saturation);

TYPED_TEST_P (Saturation, Addu) {
  constexpr auto n = TypeParam::value;
  using uint_type = sat::uinteger_t<n>;
  constexpr auto maxu = sat::mask_v<n>;
  EXPECT_EQ (sat::addu<n> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (sat::addu<n> (uint_type{2}, uint_type{3}), uint_type{5});
  EXPECT_EQ (sat::addu<n> (maxu, uint_type{1}), maxu);
}
TYPED_TEST_P (Saturation, Subu) {
  constexpr auto n = TypeParam::value;
  using uint_type = sat::uinteger_t<n>;
  constexpr auto maxu = sat::mask_v<n>;
  EXPECT_EQ (sat::subu<n> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (sat::subu<n> (uint_type{1}, uint_type{0}), uint_type{1});
  EXPECT_EQ (sat::subu<n> (uint_type{5}, uint_type{3}), uint_type{2});
  EXPECT_EQ (sat::subu<n> (uint_type{0}, uint_type{1}), uint_type{0});
  EXPECT_EQ (sat::subu<n> (maxu, uint_type{0}), maxu);
  EXPECT_EQ (sat::subu<n> (maxu, maxu), uint_type{0});
}
TYPED_TEST_P (Saturation, Divu) {
  constexpr auto bits = TypeParam::value;
  using uint_type = sat::uinteger_t<bits>;
  constexpr auto maxu = sat::mask_v<bits>;
  EXPECT_EQ (sat::divu<bits> (uint_type{0}, uint_type{3}), uint_type{0});
  EXPECT_EQ (sat::divu<bits> (uint_type{10}, uint_type{2}), uint_type{5});
  EXPECT_EQ (sat::divu<bits> (maxu, uint_type{1}), maxu);
}
TYPED_TEST_P (Saturation, Mulu) {
  constexpr auto bits = TypeParam::value;
  using uint_type = sat::uinteger_t<bits>;
  constexpr auto maxu = sat::mask_v<bits>;
  EXPECT_EQ (sat::mulu<bits> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (sat::mulu<bits> (uint_type{3}, uint_type{5}), uint_type{15});
  EXPECT_EQ (sat::mulu<bits> (maxu, uint_type{1}), maxu);
  EXPECT_EQ (sat::mulu<bits> (maxu >> 1U, uint_type{2}), uint_type{maxu - 1U});
  EXPECT_EQ (sat::mulu<bits> (maxu, uint_type{2}), maxu);
  if constexpr (bits > 16U) {
    EXPECT_EQ (sat::mulu<bits> (uint_type{13862387}, uint_type{1076719596}),
               uint_type{4294967295U & maxu});
  }
}
REGISTER_TYPED_TEST_SUITE_P (Saturation, Addu, Subu, Divu, Mulu);
template <unsigned Value>
using unsigned_constant = std::integral_constant<unsigned, Value>;
using width_types = testing::Types<unsigned_constant<9U>,
                                   unsigned_constant<15U>,
                                   unsigned_constant<16U>,
                                   unsigned_constant<17U>,
                                   unsigned_constant<24U>,
                                   unsigned_constant<31U>,
                                   unsigned_constant<32U>>;
INSTANTIATE_TYPED_TEST_SUITE_P (ExplicitWidths, Saturation, width_types, );
