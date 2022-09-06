#ifndef SUBS_HPP
#define SUBS_HPP

#include "common.hpp"

template <size_t N>
void do_test () {
#if KLEE_RUN
  std::printf ("bits=%zu: ", N);
#endif
  using sint = saturation::sinteger_t<N>;
  int exit_code = test_main<N, false> (
      [] (sint x, sint y) { return saturation::subs<N> (x, y); },
      [] (sint x, sint y) {
        return clamps<N> (static_cast<saturation::sinteger_t<N + 1>> (x) - y);
      });
  if (exit_code != EXIT_SUCCESS) {
    std::exit (exit_code);
  }
}

#endif  // SUBS_HPP
