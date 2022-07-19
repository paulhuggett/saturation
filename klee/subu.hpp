#ifndef SUBU_HPP
#define SUBU_HPP

#include "common.hpp"

template <size_t N>
void do_test () {
#if KLEE_RUN
  std::printf ("bits=%zu: ", N);
#endif
  using uinteger = saturation::uinteger_t<N>;
  int exit_code = test_main<N, true> (
      [] (uinteger x, uinteger y) { return saturation::subu<N> (x, y); },
      [] (uinteger x, uinteger y) {
        return static_cast<uinteger> (y > x ? 0 : x - y);
      });
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif  // SUBU_HPP
