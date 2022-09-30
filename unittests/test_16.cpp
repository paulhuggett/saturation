#include <gtest/gtest.h>

#include "saturation/saturation.hpp"

using namespace saturation;

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
  EXPECT_EQ (muls16 (min, 2), min);
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
