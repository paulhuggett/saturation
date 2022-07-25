#include "mulu.hpp"
int main () {
  do_test<34> ();
  do_test<36> ();
  // These tests disabled because they don't complete in a reasonable time.
  //  do_test<48> ();
  //  do_test<63> ();
  //  do_test<64> ();
}
