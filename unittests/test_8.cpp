#include <gtest/gtest.h>

#include "saturation/saturation.hpp"

using namespace saturation;

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
