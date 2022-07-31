#ifndef MULU_HPP
#define MULU_HPP

#include "common.hpp"

template <size_t N>
void do_test () {
  if constexpr (KLEE_RUN) {
    std::printf ("bits=%zu: ", N);
  }
  using uinteger = saturation::uinteger_t<N>;
  int exit_code = test_main<N, true> (
      [] (uinteger x, uinteger y) {
        if constexpr (N > 32) {
          auto const top_and_bottom_bits = 1U | (uinteger{1} << (N - 1));
          auto const masked = ~top_and_bottom_bits & saturation::mask_v<N>;
          klee_assume (!(x & masked));
        }
        return saturation::mulu<N> (x, y);
      },
      [] (uinteger x, uinteger y) {
        constexpr auto max = saturation::ulimits<N>::max ();
        auto z = static_cast<saturation::uinteger_t<N * 2>> (x) * y;
        if (z > max) {
          z = max;
        }
        return static_cast<uinteger> (z);
      });
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif  // MULU_HPP
