#ifndef DIVU_HPP
#define DIVU_HPP

#include "common.hpp"

template <size_t N>
void do_test () {
#if KLEE_RUN
  std::printf ("bits=%zu: ", N);
#endif
  using uinteger = saturation::uinteger_t<N>;
  int exit_code = test_main<N, true> (
      [] (uinteger x, uinteger y) {
        klee_assume (y != 0);
        return saturation::divu<N> (x, y);
      },
      [] (uinteger x, uinteger y) {
        // Can't overflow!
        return static_cast<uinteger> (x / y);
      });
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif  // DIVU_HPP
