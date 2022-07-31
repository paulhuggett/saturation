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

TEST (Multiply, Unsigned32) {
  using u32pair = std::pair<uint32_t, uint32_t>;
  EXPECT_EQ (details::multiply (uint32_t{1}, uint32_t{1}), (u32pair{0, 1}))
      << "1*1=1";
  EXPECT_EQ (
      details::multiply (uint32_t{0}, std::numeric_limits<uint32_t>::max ()),
      (u32pair{0, 0}))
      << "0*big=0";
  EXPECT_EQ (
      details::multiply (std::numeric_limits<uint32_t>::max (), uint32_t{0}),
      (u32pair{0, 0}))
      << "big*0 = 0";
  EXPECT_EQ (details::multiply (uint32_t{7}, uint32_t{3}), (u32pair{0, 21}))
      << "7*3 = 21";
  EXPECT_EQ (details::multiply (uint32_t{3}, uint32_t{7}), (u32pair{0, 21}))
      << "3*7 = 21";
  EXPECT_EQ (details::multiply (uint32_t{0x5555}, uint32_t{0xAAAA}),
             (u32pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (details::multiply (uint32_t{0xAAAA}, uint32_t{0x5555}),
             (u32pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (details::multiply (uint32_t{0x80000000}, uint32_t{0x80000000}),
             (u32pair{0x40000000, 0}));
  EXPECT_EQ (details::multiply (uint32_t{0x12345678}, uint32_t{0x87654321}),
             (u32pair{0x09A0CD05, 0x70B88D78}));
  EXPECT_EQ (details::multiply (uint32_t{0x87654321}, uint32_t{0x12345678}),
             (u32pair{0x09A0CD05, 0x70B88D78}));
  EXPECT_EQ (details::multiply (uint32_t{0x89ABCDEF}, uint32_t{0xFEDCBA98}),
             (u32pair{0x890F2A50, 0xAD05EBE8}));
  EXPECT_EQ (details::multiply (uint32_t{0xFEDCBA98}, uint32_t{0x89ABCDEF}),
             (u32pair{0x890F2A50, 0xAD05EBE8}));
  EXPECT_EQ (details::multiply (uint32_t{0x55555555}, uint32_t{0xAAAAAAAA}),
             (u32pair{0x38E38E38, 0x71C71C72}));
  EXPECT_EQ (details::multiply (uint32_t{0xAAAAAAAA}, uint32_t{0x55555555}),
             (u32pair{0x38E38E38, 0x71C71C72}));
  EXPECT_EQ (details::multiply (std::numeric_limits<uint32_t>::max (),
                                std::numeric_limits<uint32_t>::max ()),
             (u32pair{0xFFFFFFFE, 0x00000001}))
      << "big*big";
}

TEST (Multiply, Unsigned64) {
  using u64pair = std::pair<uint64_t, uint64_t>;
  EXPECT_EQ (details::multiply (uint64_t{1}, uint64_t{1}), (u64pair{0, 1}))
      << "1*1=1";
  EXPECT_EQ (
      details::multiply (uint64_t{0}, std::numeric_limits<uint64_t>::max ()),
      (u64pair{0, 0}))
      << "0*big=0";
  EXPECT_EQ (
      details::multiply (std::numeric_limits<uint64_t>::max (), uint64_t{0}),
      (u64pair{0, 0}))
      << "big*0 = 0";
  EXPECT_EQ (details::multiply (uint64_t{7}, uint64_t{3}), (u64pair{0, 21}))
      << "7*3 = 21";
  EXPECT_EQ (details::multiply (uint64_t{3}, uint64_t{7}), (u64pair{0, 21}))
      << "3*7 = 21";
  EXPECT_EQ (details::multiply (uint64_t{0x5555}, uint64_t{0xAAAA}),
             (u64pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (details::multiply (uint64_t{0xAAAA}, uint64_t{0x5555}),
             (u64pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (details::multiply (uint64_t{0x80000000}, uint64_t{0x80000000}),
             (u64pair{0, 0x4000000000000000}));
  EXPECT_EQ (details::multiply (uint64_t{0x12345678}, uint64_t{0x87654321}),
             (u64pair{0, 0x09A0CD0570B88D78}));
  EXPECT_EQ (details::multiply (uint64_t{0x87654321}, uint64_t{0x12345678}),
             (u64pair{0, 0x09A0CD0570B88D78}));
  EXPECT_EQ (details::multiply (uint64_t{0x89ABCDEF}, uint64_t{0xFEDCBA98}),
             (u64pair{0, 0x890F2A50AD05EBE8}));
  EXPECT_EQ (details::multiply (uint64_t{0xFEDCBA98}, uint64_t{0x89ABCDEF}),
             (u64pair{0, 0x890F2A50AD05EBE8}));
  EXPECT_EQ (details::multiply (uint64_t{0x55555555}, uint64_t{0xAAAAAAAA}),
             (u64pair{0, 0x38E38E3871C71C72}));
  EXPECT_EQ (details::multiply (uint64_t{0xAAAAAAAA}, uint64_t{0x55555555}),
             (u64pair{0, 0x38E38E3871C71C72}));
  EXPECT_EQ (details::multiply (std::numeric_limits<uint64_t>::max (),
                                std::numeric_limits<uint64_t>::max ()),
             (u64pair{0xfffffffffffffffe, 0x1}))
      << "big*big";
  // TODO: too few the tests stray into the upper 64 bits!
}

TEST (Multiply, Signed32) {
  using s32pair = std::pair<int32_t, int32_t>;
  EXPECT_EQ (details::multiply (int32_t{1}, int32_t{1}), (s32pair{0, 1}))
      << "1*1=1";
  EXPECT_EQ (details::multiply (int32_t{-1}, int32_t{-1}), (s32pair{0, 1}))
      << "-1*-1=1";
  EXPECT_EQ (details::multiply (int32_t{0}, int32_t{-1}), (s32pair{0, 0}))
      << "0*-1=0";
  EXPECT_EQ (details::multiply (int32_t{1}, int32_t{0}), (s32pair{0, 0}))
      << "-1*0=0";
  EXPECT_EQ (details::multiply (int32_t{7}, int32_t{3}), (s32pair{0, 21}))
      << "7*3=21";
  EXPECT_EQ (details::multiply (int32_t{3}, int32_t{7}), (s32pair{0, 21}))
      << "3*7=21";
  EXPECT_EQ (details::multiply (int32_t{-9}, int32_t{-33}), (s32pair{0, 297}))
      << "-9*-33=297";
  EXPECT_EQ (details::multiply (int32_t{-33}, int32_t{-9}), (s32pair{0, 297}))
      << "-33*-9=129";
  EXPECT_EQ (details::multiply (int32_t{1}, int32_t{-1}), (s32pair{-1, -1}))
      << "1*-1=-1";
  EXPECT_EQ (details::multiply (int32_t{-1}, int32_t{1}), (s32pair{-1, -1}))
      << "-1*1=-1";
  EXPECT_EQ (details::multiply (int32_t{5}, int32_t{-1}), (s32pair{-1, -5}))
      << "5*-1=-5";
  EXPECT_EQ (details::multiply (int32_t{-1}, int32_t{5}), (s32pair{-1, -5}))
      << "-1*5=-5";
  EXPECT_EQ (details::multiply (int32_t{100}, int32_t{-7}), (s32pair{-1, -700}))
      << "100*-7=-700";
  EXPECT_EQ (details::multiply (int32_t{-7}, int32_t{100}), (s32pair{-1, -700}))
      << "-7*100=-700";
  EXPECT_EQ (details::multiply (int32_t{0x5555}, int32_t{0xAAAA}),
             (s32pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (details::multiply (int32_t{0xAAAA}, int32_t{0x5555}),
             (s32pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (details::multiply (int32_t{0x100000}, int32_t{0xFEDC}),
             (s32pair{0xF, 0xEDC00000}))
      << "0x100000*0xFEDC=0xFEDC00000";
  EXPECT_EQ (details::multiply (int32_t{0xFEDC}, int32_t{0x100000}),
             (s32pair{0xF, 0xEDC00000}))
      << "0xFEDC*0x100000=0xFEDC00000";
  EXPECT_EQ (details::multiply (int32_t{0xFFFFF}, int32_t{0xEEEEE}),
             (s32pair{0xEE, 0xEED11112}))
      << "0xFFFFF*0xEEEEE=0xEEEED11112";
  EXPECT_EQ (details::multiply (int32_t{0xEEEEE}, int32_t{0xFFFFF}),
             (s32pair{0xEE, 0xEED11112}))
      << "0xEEEEE*0xFFFFF=0xEEEED11112";
  EXPECT_EQ (details::multiply (int32_t{0x7FFFFFFF}, int32_t{0x7EEEEEEE}),
             (s32pair{0x3F777776, 0x81111112}))
      << "0x7FFFFFFF*0x7EEEEEEE=0x3F77777681111112";
  EXPECT_EQ (details::multiply (int32_t{0x7EEEEEEE}, int32_t{0x7FFFFFFF}),
             (s32pair{0x3F777776, 0x81111112}))
      << "0x7EEEEEEE*0x7FFFFFFF=0x3F77777681111112";
  EXPECT_EQ (details::multiply (int32_t{-65536}, int32_t{65536}),
             (s32pair{0xFFFFFFFF, 0}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (details::multiply (int32_t{65536}, int32_t{-65536}),
             (s32pair{0xFFFFFFFF, 0}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (details::multiply (int32_t{-100'000}, int32_t{100'000}),
             (s32pair{0xFFFFFFFD, 0xABF41C00}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (details::multiply (int32_t{100'000}, int32_t{-100'000}),
             (s32pair{0xFFFFFFFD, 0xABF41C00}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (details::multiply (int32_t{-100'000}, int32_t{-150'000}),
             (s32pair{3, 0x7E11D600}))
      << "-100,000*-150,000=-15,000,000,000";
  EXPECT_EQ (details::multiply (int32_t{-150'000}, int32_t{-100'000}),
             (s32pair{3, 0x7E11D600}))
      << "-150,000*-100,000=-15,000,000,000";

  EXPECT_EQ (details::multiply (int32_t{INT32_MIN}, int32_t{INT32_MAX}), (s32pair{-1073741824, INT32_MIN}));
  EXPECT_EQ (details::multiply (int32_t{INT32_MAX}, int32_t{INT32_MIN}), (s32pair{-1073741824, INT32_MIN}));
  EXPECT_EQ(details::multiply(int32_t{-1073741824}, int32_t{-1073741824}),
             (s32pair{0x10000000, 0}));
  EXPECT_EQ (details::multiply (int32_t{0x12345678}, int32_t{-2023406815}),
             (s32pair{-143886707, 1891143032}));
  EXPECT_EQ (details::multiply (int32_t{-2023406815}, int32_t{0x12345678}),
             (s32pair{-143886707, 1891143032}));
  EXPECT_EQ (details::multiply (int32_t{-1985229329}, int32_t{-19088744}),
             (s32pair{8823241, -1392120856}));
  EXPECT_EQ (details::multiply (int32_t{-19088744}, int32_t{-1985229329}),
             (s32pair{8823241, -1392120856}));
  EXPECT_EQ (details::multiply (int32_t{0x55555555}, int32_t{-1431655766}),
             (s32pair{-477218589, 0x71C71C72}));
  EXPECT_EQ (details::multiply (int32_t{0x55555555}, int32_t{-1431655766}),
             (s32pair{-477218589, 0x71C71C72}));
}

TEST (Multiply, Signed64) {
  using s64pair = std::pair<int64_t, int64_t>;
  EXPECT_EQ (details::multiply (int64_t{1}, int64_t{1}), (s64pair{0, 1}))
      << "1*1=1";
  EXPECT_EQ (details::multiply (int64_t{-1}, int64_t{-1}), (s64pair{0, 1}))
      << "-1*-1=1";
  EXPECT_EQ (details::multiply (int64_t{0}, int64_t{-1}), (s64pair{0, 0}))
      << "0*-1=0";
  EXPECT_EQ (details::multiply (int64_t{1}, int64_t{0}), (s64pair{0, 0}))
      << "-1*0=0";
  EXPECT_EQ (details::multiply (int64_t{7}, int64_t{3}), (s64pair{0, 21}))
      << "7*3=21";
  EXPECT_EQ (details::multiply (int64_t{3}, int64_t{7}), (s64pair{0, 21}))
      << "3*7=21";
  EXPECT_EQ (details::multiply (int64_t{-9}, int64_t{-33}), (s64pair{0, 297}))
      << "-9*-33=297";
  EXPECT_EQ (details::multiply (int64_t{-33}, int64_t{-9}), (s64pair{0, 297}))
      << "-33*-9=129";
  EXPECT_EQ (details::multiply (int64_t{1}, int64_t{-1}), (s64pair{-1, -1}))
      << "1*-1=-1";
  EXPECT_EQ (details::multiply (int64_t{-1}, int64_t{1}), (s64pair{-1, -1}))
      << "-1*1=-1";
  EXPECT_EQ (details::multiply (int64_t{5}, int64_t{-1}), (s64pair{-1, -5}))
      << "5*-1=-5";
  EXPECT_EQ (details::multiply (int64_t{-1}, int64_t{5}), (s64pair{-1, -5}))
      << "-1*5=-5";
  EXPECT_EQ (details::multiply (int64_t{100}, int64_t{-7}), (s64pair{-1, -700}))
      << "100*-7=-700";
  EXPECT_EQ (details::multiply (int64_t{-7}, int64_t{100}), (s64pair{-1, -700}))
      << "-7*100=-700";
  EXPECT_EQ (details::multiply (int64_t{0x5555}, int64_t{0xAAAA}),
             (s64pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (details::multiply (int64_t{0xAAAA}, int64_t{0x5555}),
             (s64pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (details::multiply (int64_t{0x100000}, int64_t{0xFEDC}),
             (s64pair{0, 0xFEDC00000}))
      << "0x100000*0xFEDC=0xFEDC00000";
  EXPECT_EQ (details::multiply (int64_t{0xFEDC}, int64_t{0x100000}),
             (s64pair{0, 0xFEDC00000}))
      << "0xFEDC*0x100000=0xFEDC00000";
  EXPECT_EQ (details::multiply (int64_t{0xFFFFF}, int64_t{0xEEEEE}),
             (s64pair{0, 0xEEEED11112}))
      << "0xFFFFF*0xEEEEE=0xEEEED11112";
  EXPECT_EQ (details::multiply (int64_t{0xEEEEE}, int64_t{0xFFFFF}),
             (s64pair{0, 0xEEEED11112}))
      << "0xEEEEE*0xFFFFF=0xEEEED11112";
  EXPECT_EQ (details::multiply (int64_t{0x7FFFFFFF}, int64_t{0x7EEEEEEE}),
             (s64pair{0, 0x3F77777681111112}))
      << "0x7FFFFFFF*0x7EEEEEEE=0x3F77777681111112";
  EXPECT_EQ (details::multiply (int64_t{0x7EEEEEEE}, int64_t{0x7FFFFFFF}),
             (s64pair{0, 0x3F77777681111112}))
      << "0x7EEEEEEE*0x7FFFFFFF=0x3F77777681111112";
  EXPECT_EQ (details::multiply (int64_t{-65536}, int64_t{65536}),
             (s64pair{-1, 0xFFFFFFFF00000000}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (details::multiply (int64_t{65536}, int64_t{-65536}),
             (s64pair{-1, 0xFFFFFFFF00000000}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (details::multiply (int64_t{-100'000}, int64_t{100'000}),
             (s64pair{-1, -10000000000}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (details::multiply (int64_t{100'000}, int64_t{-100'000}),
             (s64pair{-1, -10000000000}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (details::multiply (int64_t{-100'000}, int64_t{-150'000}),
             (s64pair{0, 15'000'000'000}))
      << "-100,000*-150,000=-15,000,000,000";
  EXPECT_EQ (details::multiply (int64_t{-150'000}, int64_t{-100'000}),
             (s64pair{0, 15'000'000'000}))
      << "-150,000*-100,000=-15,000,000,000";
  EXPECT_EQ (details::multiply (int64_t{INT32_MIN}, int64_t{INT32_MAX}),
             (s64pair{-1, 0xc000000080000000}));
  EXPECT_EQ (details::multiply (int64_t{INT32_MAX}, int64_t{INT32_MIN}),
             (s64pair{-1, 0xc000000080000000}));
  EXPECT_EQ (details::multiply (int64_t{-1073741824}, int64_t{-1073741824}),
             (s64pair{0, 0x1000000000000000}));
  EXPECT_EQ (details::multiply (int64_t{0x12345678}, int64_t{-2023406815}),
             (s64pair{-1, 0xf76c768d70b88d78}));
  EXPECT_EQ (details::multiply (int64_t{-2023406815}, int64_t{0x12345678}),
             (s64pair{-1, 0xf76c768d70b88d78}));
  EXPECT_EQ (details::multiply (int64_t{-1985229329}, int64_t{-19088744}),
             (s64pair{0, 37895534442572776}));
  EXPECT_EQ (details::multiply (int64_t{-19088744}, int64_t{-1985229329}),
             (s64pair{0, 37895534442572776}));
  EXPECT_EQ (details::multiply (int64_t{1431655765}, int64_t{-1431655766}),
             (s64pair{-1, -2049638230889390990}));
  EXPECT_EQ (details::multiply (int64_t{1431655765}, int64_t{-1431655766}),
             (s64pair{-1, -2049638230889390990}));

  EXPECT_EQ (details::multiply (int64_t{INT64_MAX}, int64_t{INT64_MAX}),
             (s64pair{0x3FFFFFFFFFFFFFFF, 0x0000000000000001}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{INT64_MIN}),
             (s64pair{0x4000000000000000, 0x0000000000000000}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{INT64_MAX}),
             (s64pair{0xc000000000000000, 0x8000000000000000}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MAX}, int64_t{INT64_MIN}),
             (s64pair{0xc000000000000000, 0x8000000000000000}));

  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{-1}),
             (s64pair{0, 0x8000000000000000}));
  EXPECT_EQ (details::multiply (int64_t{-1}, int64_t{INT64_MIN}),
             (s64pair{0, 0x8000000000000000}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{-2}),
             (s64pair{1, 0}));
  EXPECT_EQ (details::multiply (int64_t{-2}, int64_t{INT64_MIN}),
             (s64pair{1, 0}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{0}),
             (s64pair{0, 0}));
  EXPECT_EQ (details::multiply (int64_t{0}, int64_t{INT64_MIN}),
             (s64pair{0, 0}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{1}),
             (s64pair{-1, INT64_MIN}));
  EXPECT_EQ (details::multiply (int64_t{1}, int64_t{INT64_MIN}),
             (s64pair{-1, INT64_MIN}));
  EXPECT_EQ (details::multiply (int64_t{INT64_MIN}, int64_t{2}),
             (s64pair{-1, 0}));
  EXPECT_EQ (details::multiply (int64_t{2}, int64_t{INT64_MIN}),
             (s64pair{-1, 0}));
}

TEST (Mulu, 48) {
  using uint_type = uinteger_t<48>;
  EXPECT_EQ (mulu<48> (uint_type{0x800000000000}, uint_type{0x000100000000}),
             uint_type{0xFFFFFFFFFFFF});
  EXPECT_EQ (mulu<48> (uint_type{140737488355328}, uint_type{1099511627776}),
             uint_type{281474976710655});
}

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
  constexpr auto max = limits<bits>::max ();
  constexpr auto min = limits<bits>::min ();
  EXPECT_EQ (muls<bits> (sint_type{0}, sint_type{0}), sint_type{0});
  EXPECT_EQ (muls<bits> (sint_type{-1}, sint_type{-1}), sint_type{1});
  EXPECT_EQ (muls<bits> (sint_type{1}, sint_type{-1}), sint_type{-1});
  EXPECT_EQ (muls<bits> (sint_type{-1}, sint_type{1}), sint_type{-1});
  EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{2}), sint_type{6});
  EXPECT_EQ (muls<bits> (sint_type{-3}, sint_type{2}), sint_type{-6});
  EXPECT_EQ (muls<bits> (sint_type{3}, sint_type{-2}), sint_type{-6});
  EXPECT_EQ (muls<bits> (max, sint_type{1}), max);
  EXPECT_EQ (muls<bits> (max, sint_type{2}), max);
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
