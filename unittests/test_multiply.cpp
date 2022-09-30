#include <gtest/gtest.h>

#include "saturation/mul.hpp"

using saturation::details::multiply;

TEST (Multiply, Unsigned32) {
  using u32pair = std::pair<uint32_t, uint32_t>;
  EXPECT_EQ (multiply (uint32_t{1}, uint32_t{1}), (u32pair{0, 1})) << "1*1=1";
  EXPECT_EQ (multiply (uint32_t{0}, std::numeric_limits<uint32_t>::max ()),
             (u32pair{0, 0}))
      << "0*big=0";
  EXPECT_EQ (multiply (std::numeric_limits<uint32_t>::max (), uint32_t{0}),
             (u32pair{0, 0}))
      << "big*0 = 0";
  EXPECT_EQ (multiply (uint32_t{7}, uint32_t{3}), (u32pair{0, 21}))
      << "7*3 = 21";
  EXPECT_EQ (multiply (uint32_t{3}, uint32_t{7}), (u32pair{0, 21}))
      << "3*7 = 21";
  EXPECT_EQ (multiply (uint32_t{0x5555}, uint32_t{0xAAAA}),
             (u32pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (multiply (uint32_t{0xAAAA}, uint32_t{0x5555}),
             (u32pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (multiply (uint32_t{0x80000000}, uint32_t{0x80000000}),
             (u32pair{0x40000000, 0}));
  EXPECT_EQ (multiply (uint32_t{0x12345678}, uint32_t{0x87654321}),
             (u32pair{0x09A0CD05, 0x70B88D78}));
  EXPECT_EQ (multiply (uint32_t{0x87654321}, uint32_t{0x12345678}),
             (u32pair{0x09A0CD05, 0x70B88D78}));
  EXPECT_EQ (multiply (uint32_t{0x89ABCDEF}, uint32_t{0xFEDCBA98}),
             (u32pair{0x890F2A50, 0xAD05EBE8}));
  EXPECT_EQ (multiply (uint32_t{0xFEDCBA98}, uint32_t{0x89ABCDEF}),
             (u32pair{0x890F2A50, 0xAD05EBE8}));
  EXPECT_EQ (multiply (uint32_t{0x55555555}, uint32_t{0xAAAAAAAA}),
             (u32pair{0x38E38E38, 0x71C71C72}));
  EXPECT_EQ (multiply (uint32_t{0xAAAAAAAA}, uint32_t{0x55555555}),
             (u32pair{0x38E38E38, 0x71C71C72}));
  EXPECT_EQ (multiply (std::numeric_limits<uint32_t>::max (),
                       std::numeric_limits<uint32_t>::max ()),
             (u32pair{0xFFFFFFFE, 0x00000001}))
      << "big*big";
}

TEST (Multiply, Unsigned64) {
  using u64pair = std::pair<uint64_t, uint64_t>;
  EXPECT_EQ (multiply (uint64_t{1}, uint64_t{1}), (u64pair{0, 1})) << "1*1=1";
  EXPECT_EQ (multiply (uint64_t{0}, std::numeric_limits<uint64_t>::max ()),
             (u64pair{0, 0}))
      << "0*big=0";
  EXPECT_EQ (multiply (std::numeric_limits<uint64_t>::max (), uint64_t{0}),
             (u64pair{0, 0}))
      << "big*0 = 0";
  EXPECT_EQ (multiply (uint64_t{7}, uint64_t{3}), (u64pair{0, 21}))
      << "7*3 = 21";
  EXPECT_EQ (multiply (uint64_t{3}, uint64_t{7}), (u64pair{0, 21}))
      << "3*7 = 21";
  EXPECT_EQ (multiply (uint64_t{0x5555}, uint64_t{0xAAAA}),
             (u64pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (multiply (uint64_t{0xAAAA}, uint64_t{0x5555}),
             (u64pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (multiply (uint64_t{0x80000000}, uint64_t{0x80000000}),
             (u64pair{0, 0x4000000000000000}));
  EXPECT_EQ (multiply (uint64_t{0x12345678}, uint64_t{0x87654321}),
             (u64pair{0, 0x09A0CD0570B88D78}));
  EXPECT_EQ (multiply (uint64_t{0x87654321}, uint64_t{0x12345678}),
             (u64pair{0, 0x09A0CD0570B88D78}));
  EXPECT_EQ (multiply (uint64_t{0x89ABCDEF}, uint64_t{0xFEDCBA98}),
             (u64pair{0, 0x890F2A50AD05EBE8}));
  EXPECT_EQ (multiply (uint64_t{0xFEDCBA98}, uint64_t{0x89ABCDEF}),
             (u64pair{0, 0x890F2A50AD05EBE8}));
  EXPECT_EQ (multiply (uint64_t{0x55555555}, uint64_t{0xAAAAAAAA}),
             (u64pair{0, 0x38E38E3871C71C72}));
  EXPECT_EQ (multiply (uint64_t{0xAAAAAAAA}, uint64_t{0x55555555}),
             (u64pair{0, 0x38E38E3871C71C72}));
  EXPECT_EQ (multiply (std::numeric_limits<uint64_t>::max (),
                       std::numeric_limits<uint64_t>::max ()),
             (u64pair{0xfffffffffffffffe, 0x1}))
      << "big*big";
}

TEST (Multiply, Signed32) {
  using s32pair = std::pair<int32_t, int32_t>;
  EXPECT_EQ (multiply (int32_t{1}, int32_t{1}), (s32pair{0, 1})) << "1*1=1";
  EXPECT_EQ (multiply (int32_t{-1}, int32_t{-1}), (s32pair{0, 1})) << "-1*-1=1";
  EXPECT_EQ (multiply (int32_t{0}, int32_t{-1}), (s32pair{0, 0})) << "0*-1=0";
  EXPECT_EQ (multiply (int32_t{1}, int32_t{0}), (s32pair{0, 0})) << "-1*0=0";
  EXPECT_EQ (multiply (int32_t{7}, int32_t{3}), (s32pair{0, 21})) << "7*3=21";
  EXPECT_EQ (multiply (int32_t{3}, int32_t{7}), (s32pair{0, 21})) << "3*7=21";
  EXPECT_EQ (multiply (int32_t{-9}, int32_t{-33}), (s32pair{0, 297}))
      << "-9*-33=297";
  EXPECT_EQ (multiply (int32_t{-33}, int32_t{-9}), (s32pair{0, 297}))
      << "-33*-9=129";
  EXPECT_EQ (multiply (int32_t{1}, int32_t{-1}), (s32pair{-1, -1}))
      << "1*-1=-1";
  EXPECT_EQ (multiply (int32_t{-1}, int32_t{1}), (s32pair{-1, -1}))
      << "-1*1=-1";
  EXPECT_EQ (multiply (int32_t{5}, int32_t{-1}), (s32pair{-1, -5}))
      << "5*-1=-5";
  EXPECT_EQ (multiply (int32_t{-1}, int32_t{5}), (s32pair{-1, -5}))
      << "-1*5=-5";
  EXPECT_EQ (multiply (int32_t{100}, int32_t{-7}), (s32pair{-1, -700}))
      << "100*-7=-700";
  EXPECT_EQ (multiply (int32_t{-7}, int32_t{100}), (s32pair{-1, -700}))
      << "-7*100=-700";
  EXPECT_EQ (multiply (int32_t{0x5555}, int32_t{0xAAAA}),
             (s32pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (multiply (int32_t{0xAAAA}, int32_t{0x5555}),
             (s32pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (multiply (int32_t{0x100000}, int32_t{0xFEDC}),
             (s32pair{0xF, 0xEDC00000}))
      << "0x100000*0xFEDC=0xFEDC00000";
  EXPECT_EQ (multiply (int32_t{0xFEDC}, int32_t{0x100000}),
             (s32pair{0xF, 0xEDC00000}))
      << "0xFEDC*0x100000=0xFEDC00000";
  EXPECT_EQ (multiply (int32_t{0xFFFFF}, int32_t{0xEEEEE}),
             (s32pair{0xEE, 0xEED11112}))
      << "0xFFFFF*0xEEEEE=0xEEEED11112";
  EXPECT_EQ (multiply (int32_t{0xEEEEE}, int32_t{0xFFFFF}),
             (s32pair{0xEE, 0xEED11112}))
      << "0xEEEEE*0xFFFFF=0xEEEED11112";
  EXPECT_EQ (multiply (int32_t{0x7FFFFFFF}, int32_t{0x7EEEEEEE}),
             (s32pair{0x3F777776, 0x81111112}))
      << "0x7FFFFFFF*0x7EEEEEEE=0x3F77777681111112";
  EXPECT_EQ (multiply (int32_t{0x7EEEEEEE}, int32_t{0x7FFFFFFF}),
             (s32pair{0x3F777776, 0x81111112}))
      << "0x7EEEEEEE*0x7FFFFFFF=0x3F77777681111112";
  EXPECT_EQ (multiply (int32_t{-65536}, int32_t{65536}),
             (s32pair{0xFFFFFFFF, 0}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (multiply (int32_t{65536}, int32_t{-65536}),
             (s32pair{0xFFFFFFFF, 0}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (multiply (int32_t{-100'000}, int32_t{100'000}),
             (s32pair{0xFFFFFFFD, 0xABF41C00}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (multiply (int32_t{100'000}, int32_t{-100'000}),
             (s32pair{0xFFFFFFFD, 0xABF41C00}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (multiply (int32_t{-100'000}, int32_t{-150'000}),
             (s32pair{3, 0x7E11D600}))
      << "-100,000*-150,000=-15,000,000,000";
  EXPECT_EQ (multiply (int32_t{-150'000}, int32_t{-100'000}),
             (s32pair{3, 0x7E11D600}))
      << "-150,000*-100,000=-15,000,000,000";

  EXPECT_EQ (multiply (int32_t{INT32_MIN}, int32_t{INT32_MAX}),
             (s32pair{-1073741824, INT32_MIN}));
  EXPECT_EQ (multiply (int32_t{INT32_MAX}, int32_t{INT32_MIN}),
             (s32pair{-1073741824, INT32_MIN}));
  EXPECT_EQ (multiply (int32_t{-1073741824}, int32_t{-1073741824}),
             (s32pair{0x10000000, 0}));
  EXPECT_EQ (multiply (int32_t{0x12345678}, int32_t{-2023406815}),
             (s32pair{-143886707, 1891143032}));
  EXPECT_EQ (multiply (int32_t{-2023406815}, int32_t{0x12345678}),
             (s32pair{-143886707, 1891143032}));
  EXPECT_EQ (multiply (int32_t{-1985229329}, int32_t{-19088744}),
             (s32pair{8823241, -1392120856}));
  EXPECT_EQ (multiply (int32_t{-19088744}, int32_t{-1985229329}),
             (s32pair{8823241, -1392120856}));
  EXPECT_EQ (multiply (int32_t{0x55555555}, int32_t{-1431655766}),
             (s32pair{-477218589, 0x71C71C72}));
  EXPECT_EQ (multiply (int32_t{0x55555555}, int32_t{-1431655766}),
             (s32pair{-477218589, 0x71C71C72}));
}

TEST (Multiply, Signed64) {
  using s64pair = std::pair<int64_t, int64_t>;
  EXPECT_EQ (multiply (int64_t{1}, int64_t{1}), (s64pair{0, 1})) << "1*1=1";
  EXPECT_EQ (multiply (int64_t{-1}, int64_t{-1}), (s64pair{0, 1})) << "-1*-1=1";
  EXPECT_EQ (multiply (int64_t{0}, int64_t{-1}), (s64pair{0, 0})) << "0*-1=0";
  EXPECT_EQ (multiply (int64_t{1}, int64_t{0}), (s64pair{0, 0})) << "-1*0=0";
  EXPECT_EQ (multiply (int64_t{7}, int64_t{3}), (s64pair{0, 21})) << "7*3=21";
  EXPECT_EQ (multiply (int64_t{3}, int64_t{7}), (s64pair{0, 21})) << "3*7=21";
  EXPECT_EQ (multiply (int64_t{-9}, int64_t{-33}), (s64pair{0, 297}))
      << "-9*-33=297";
  EXPECT_EQ (multiply (int64_t{-33}, int64_t{-9}), (s64pair{0, 297}))
      << "-33*-9=129";
  EXPECT_EQ (multiply (int64_t{1}, int64_t{-1}), (s64pair{-1, -1}))
      << "1*-1=-1";
  EXPECT_EQ (multiply (int64_t{-1}, int64_t{1}), (s64pair{-1, -1}))
      << "-1*1=-1";
  EXPECT_EQ (multiply (int64_t{5}, int64_t{-1}), (s64pair{-1, -5}))
      << "5*-1=-5";
  EXPECT_EQ (multiply (int64_t{-1}, int64_t{5}), (s64pair{-1, -5}))
      << "-1*5=-5";
  EXPECT_EQ (multiply (int64_t{100}, int64_t{-7}), (s64pair{-1, -700}))
      << "100*-7=-700";
  EXPECT_EQ (multiply (int64_t{-7}, int64_t{100}), (s64pair{-1, -700}))
      << "-7*100=-700";
  EXPECT_EQ (multiply (int64_t{0x5555}, int64_t{0xAAAA}),
             (s64pair{0, 0x38E31C72}))
      << "0x5555*0xAAAA=0x38E31C72";
  EXPECT_EQ (multiply (int64_t{0xAAAA}, int64_t{0x5555}),
             (s64pair{0, 0x38E31C72}))
      << "0xAAAA*0x5555=0x38E31C72";
  EXPECT_EQ (multiply (int64_t{0x100000}, int64_t{0xFEDC}),
             (s64pair{0, 0xFEDC00000}))
      << "0x100000*0xFEDC=0xFEDC00000";
  EXPECT_EQ (multiply (int64_t{0xFEDC}, int64_t{0x100000}),
             (s64pair{0, 0xFEDC00000}))
      << "0xFEDC*0x100000=0xFEDC00000";
  EXPECT_EQ (multiply (int64_t{0xFFFFF}, int64_t{0xEEEEE}),
             (s64pair{0, 0xEEEED11112}))
      << "0xFFFFF*0xEEEEE=0xEEEED11112";
  EXPECT_EQ (multiply (int64_t{0xEEEEE}, int64_t{0xFFFFF}),
             (s64pair{0, 0xEEEED11112}))
      << "0xEEEEE*0xFFFFF=0xEEEED11112";
  EXPECT_EQ (multiply (int64_t{0x7FFFFFFF}, int64_t{0x7EEEEEEE}),
             (s64pair{0, 0x3F77777681111112}))
      << "0x7FFFFFFF*0x7EEEEEEE=0x3F77777681111112";
  EXPECT_EQ (multiply (int64_t{0x7EEEEEEE}, int64_t{0x7FFFFFFF}),
             (s64pair{0, 0x3F77777681111112}))
      << "0x7EEEEEEE*0x7FFFFFFF=0x3F77777681111112";
  EXPECT_EQ (multiply (int64_t{-65536}, int64_t{65536}),
             (s64pair{-1, 0xFFFFFFFF00000000}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (multiply (int64_t{65536}, int64_t{-65536}),
             (s64pair{-1, 0xFFFFFFFF00000000}))
      << "-65536*65536=-4294967296";
  EXPECT_EQ (multiply (int64_t{-100'000}, int64_t{100'000}),
             (s64pair{-1, -10000000000}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (multiply (int64_t{100'000}, int64_t{-100'000}),
             (s64pair{-1, -10000000000}))
      << "-100,000*100,000=10,000,000,000";
  EXPECT_EQ (multiply (int64_t{-100'000}, int64_t{-150'000}),
             (s64pair{0, 15'000'000'000}))
      << "-100,000*-150,000=-15,000,000,000";
  EXPECT_EQ (multiply (int64_t{-150'000}, int64_t{-100'000}),
             (s64pair{0, 15'000'000'000}))
      << "-150,000*-100,000=-15,000,000,000";
  EXPECT_EQ (multiply (int64_t{INT32_MIN}, int64_t{INT32_MAX}),
             (s64pair{-1, 0xc000000080000000}));
  EXPECT_EQ (multiply (int64_t{INT32_MAX}, int64_t{INT32_MIN}),
             (s64pair{-1, 0xc000000080000000}));
  EXPECT_EQ (multiply (int64_t{-1073741824}, int64_t{-1073741824}),
             (s64pair{0, 0x1000000000000000}));
  EXPECT_EQ (multiply (int64_t{0x12345678}, int64_t{-2023406815}),
             (s64pair{-1, 0xf76c768d70b88d78}));
  EXPECT_EQ (multiply (int64_t{-2023406815}, int64_t{0x12345678}),
             (s64pair{-1, 0xf76c768d70b88d78}));
  EXPECT_EQ (multiply (int64_t{-1985229329}, int64_t{-19088744}),
             (s64pair{0, 37895534442572776}));
  EXPECT_EQ (multiply (int64_t{-19088744}, int64_t{-1985229329}),
             (s64pair{0, 37895534442572776}));
  EXPECT_EQ (multiply (int64_t{1431655765}, int64_t{-1431655766}),
             (s64pair{-1, -2049638230889390990}));
  EXPECT_EQ (multiply (int64_t{1431655765}, int64_t{-1431655766}),
             (s64pair{-1, -2049638230889390990}));

  EXPECT_EQ (multiply (int64_t{INT64_MAX}, int64_t{INT64_MAX}),
             (s64pair{0x3FFFFFFFFFFFFFFF, 0x0000000000000001}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{INT64_MIN}),
             (s64pair{0x4000000000000000, 0x0000000000000000}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{INT64_MAX}),
             (s64pair{0xc000000000000000, 0x8000000000000000}));
  EXPECT_EQ (multiply (int64_t{INT64_MAX}, int64_t{INT64_MIN}),
             (s64pair{0xc000000000000000, 0x8000000000000000}));

  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{-1}),
             (s64pair{0, 0x8000000000000000}));
  EXPECT_EQ (multiply (int64_t{-1}, int64_t{INT64_MIN}),
             (s64pair{0, 0x8000000000000000}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{-2}), (s64pair{1, 0}));
  EXPECT_EQ (multiply (int64_t{-2}, int64_t{INT64_MIN}), (s64pair{1, 0}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{0}), (s64pair{0, 0}));
  EXPECT_EQ (multiply (int64_t{0}, int64_t{INT64_MIN}), (s64pair{0, 0}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{1}),
             (s64pair{-1, INT64_MIN}));
  EXPECT_EQ (multiply (int64_t{1}, int64_t{INT64_MIN}),
             (s64pair{-1, INT64_MIN}));
  EXPECT_EQ (multiply (int64_t{INT64_MIN}, int64_t{2}), (s64pair{-1, 0}));
  EXPECT_EQ (multiply (int64_t{2}, int64_t{INT64_MIN}), (s64pair{-1, 0}));
}
