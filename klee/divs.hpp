#ifndef DIVS_HPP
#define DIVS_HPP

#include "common.hpp"

template <size_t N>
void limit_tests (saturation::sinteger_t<N> x) {
  if constexpr (N > 8) {
    // Reduce the number of generated test cases by forcing the state of
    // some bits.
    using uinteger = saturation::uinteger_t<N>;
    auto const top_and_bottom_bits = 1U | (uinteger{1} << (N - 2));
    auto const masked = ~top_and_bottom_bits & saturation::mask_v<N>;
    klee_assume (!(static_cast<uinteger> (x) & masked));
  }
}

template <size_t N>
void do_test () {
  if constexpr (KLEE_RUN) {
    std::printf ("bits=%zu: ", N);
  }
  using sinteger = saturation::sinteger_t<N>;
  int exit_code = test_main<N, false> (
      [] (sinteger x, sinteger y) {
        limit_tests<N> (x);
        return saturation::divs<N> (x, y);
      },
      [] (sinteger x, sinteger y) { return clamps<N> (x / y); },
      false  // Ensure y != 0
  );
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif  // DIVS_HPP
