// Test unsigned add for [4,5] and [7,8,9] bit values.
#include "addu.hpp"
int main () {
  do_test<4> ();
  do_test<5> ();
  do_test<7> ();
  do_test<8> ();
  do_test<9> ();
}
