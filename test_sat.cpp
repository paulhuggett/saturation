#include <gtest/gtest.h>

#include "saturation.hpp"

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
  constexpr auto n = TypeParam::value;
  using uint_type = sat::uinteger_t<n>;
  constexpr auto maxu = sat::mask_v<n>;
  EXPECT_EQ (sat::divu<n> (uint_type{0}, uint_type{3}), uint_type{0});
  EXPECT_EQ (sat::divu<n> (uint_type{10}, uint_type{2}), uint_type{5});
  EXPECT_EQ (sat::divu<n> (maxu, uint_type{1}), maxu);
}
TYPED_TEST_P (Saturation, Mulu) {
  constexpr auto n = TypeParam::value;
  using uint_type = sat::uinteger_t<n>;
  EXPECT_EQ (sat::mulu<n> (uint_type{0}, uint_type{0}), uint_type{0});
  EXPECT_EQ (sat::mulu<n> (uint_type{3}, uint_type{5}), uint_type{15});
  EXPECT_EQ (sat::mulu<n> (sat::mask_v<n>, uint_type{1}), sat::mask_v<n>);
  EXPECT_EQ (sat::mulu<n> (sat::mask_v<n> >> 1U, uint_type{2}),
             uint_type{sat::mask_v<n> - 1U});
  if constexpr (n > 16U) {
    EXPECT_EQ (sat::mulu<n> (uint_type{13862387}, uint_type{1076719596}),
               uint_type{4294967295} & sat::mask_v<n>);
  }
}
REGISTER_TYPED_TEST_SUITE_P (Saturation, Addu, Subu, Divu, Mulu);
using width_types = testing::Types<std::integral_constant<unsigned, 9U>,
                                   std::integral_constant<unsigned, 15U>,
                                   std::integral_constant<unsigned, 16U>,
                                   std::integral_constant<unsigned, 17U>,
                                   std::integral_constant<unsigned, 24U>,
                                   std::integral_constant<unsigned, 31U>,
                                   std::integral_constant<unsigned, 32U>>;
INSTANTIATE_TYPED_TEST_SUITE_P (ExplicitWidths, Saturation, width_types, );
