#include <gtest/gtest.h>

#include "sat.hpp"

TEST (Saturation, Adds32) {
  constexpr auto min = std::numeric_limits<int32_t>::min();
  constexpr auto max = std::numeric_limits<int32_t>::max();
  EXPECT_EQ (sat::adds32(int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::adds32(int32_t{3}, int32_t{5}), int32_t{8});
  EXPECT_EQ (sat::adds32(min, min), min);
  EXPECT_EQ (sat::adds32(max, max), max);
  EXPECT_EQ (sat::adds32(int32_t{16777216}, int32_t{2130706432}), int32_t{2147483647});
}

TEST (Saturation, Subs32) {
  constexpr auto min = std::numeric_limits<int32_t>::min();
  constexpr auto max = std::numeric_limits<int32_t>::max();
  EXPECT_EQ (sat::subs32(int32_t{0}, int32_t{0}), int32_t{0});
  EXPECT_EQ (sat::subs32(min, int32_t{1}), min);
  EXPECT_EQ (sat::subs32(0, min), max);
  EXPECT_EQ (sat::subs32(max, max), int32_t{0});
}
