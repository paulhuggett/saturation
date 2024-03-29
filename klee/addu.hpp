#ifndef ADDU_HPP
#define ADDU_HPP

#include "common.hpp"

template <size_t N>
void do_test () {
#if KLEE_RUN
  std::printf ("bits=%zu: ", N);
#endif
  using uinteger = saturation::uinteger_t<N>;
  int exit_code = test_main<N, true> (
      [] (uinteger x, uinteger y) { return saturation::addu<N> (x, y); },
      [] (uinteger x, uinteger y) {
        return clampu<N> (static_cast<saturation::uinteger_t<N + 1>> (x) + y);
      });
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif // ADDU_HPP
