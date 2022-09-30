#include <gtest/gtest.h>

#include "saturation/saturation.hpp"

using namespace saturation;

class Saturation32 : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int32_t>::min ();
  static constexpr auto max = std::numeric_limits<int32_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint32_t>::max ();
};

TEST_F (Saturation32, SignedAdd) {
  EXPECT_EQ (adds32 (0, 0), 0);
  EXPECT_EQ (adds32 (3, 5), 8);
  EXPECT_EQ (adds32 (min, 1), min + 1);
  EXPECT_EQ (adds32 (min, -1), min);
  EXPECT_EQ (adds32 (min, min), min);
  EXPECT_EQ (adds32 (max, max), max);
  EXPECT_EQ (adds32 (16777216, 2130706432), max);
}
TEST_F (Saturation32, SignedSubtract) {
  EXPECT_EQ (subs32 (0, 0), 0);
  EXPECT_EQ (subs32 (0, 1), -1);
  EXPECT_EQ (subs32 (min, 1), min);
  EXPECT_EQ (subs32 (min, -1), min + 1);
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
