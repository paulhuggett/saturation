#include <gtest/gtest.h>

#include "sat.hpp"

class Saturation : public testing::Test {
protected:
  static constexpr auto min = std::numeric_limits<int32_t>::min ();
  static constexpr auto max = std::numeric_limits<int32_t>::max ();
  static constexpr auto maxu = std::numeric_limits<uint32_t>::max ();
};

TEST_F (Saturation, Adds32) {
  EXPECT_EQ (sat::adds32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::adds32 (int32_t{3}, int32_t{5}), int32_t{8});
  EXPECT_EQ (sat::adds32 (min, min), min);
  EXPECT_EQ (sat::adds32 (max, max), max);
  EXPECT_EQ (sat::adds32 (int32_t{16777216}, int32_t{2130706432}), max);
}

TEST_F (Saturation, Subs32) {
  EXPECT_EQ (sat::subs32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::subs32 (int32_t{0}, int32_t{1}), int32_t{-1});
  EXPECT_EQ (sat::subs32 (min, int32_t{1}), min);
  EXPECT_EQ (sat::subs32 (0, min), max);
  EXPECT_EQ (sat::subs32 (max, int32_t{0}), max);
  EXPECT_EQ (sat::subs32 (max, max), int32_t{0});
  EXPECT_EQ (sat::subs32 (max, int32_t{1}), int32_t{max - 1});
}

TEST_F (Saturation, Divs32) {
  EXPECT_EQ (sat::divs32 (int32_t{0}, int32_t{1}), int32_t{0});
  EXPECT_EQ (sat::divs32 (int32_t{4}, int32_t{2}), int32_t{2});
  EXPECT_EQ (sat::divs32 (max, int32_t{2}), int32_t{max / 2});
  EXPECT_EQ (sat::divs32 (min, int32_t{-1}), max);
}

TEST_F (Saturation, Muls32) {
  EXPECT_EQ (sat::muls32 (int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{5}), int32_t{15});
  EXPECT_EQ (sat::muls32 (int32_t{-3}, int32_t{5}), int32_t{-15});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{-5}), int32_t{-15});
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{-1342177280}), min);
  EXPECT_EQ (sat::muls32 (int32_t{3}, int32_t{997556224}), max);
}

TEST_F (Saturation, Addu32) {
  EXPECT_EQ (sat::addu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::addu32 (maxu, uint32_t{1}), maxu);
}

TEST_F (Saturation, Subu32) {
  EXPECT_EQ (sat::subu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::subu32 (uint32_t{0}, uint32_t{1}), uint32_t{0});
}

TEST_F (Saturation, Divu32) {
  EXPECT_EQ (sat::divu32 (uint32_t{0}, uint32_t{16843009}), uint32_t{0});
}

TEST_F (Saturation, Mulu32) {
  EXPECT_EQ (sat::mulu32 (uint32_t{0}, uint32_t{0}), uint32_t{0});
  EXPECT_EQ (sat::mulu32 (uint32_t{3}, uint32_t{5}), uint32_t{15});
  EXPECT_EQ (sat::mulu32 (uint32_t{13862387}, uint32_t{1076719596}),
             uint32_t{4294967295});
}
