// Test unsigned subtract for [15,17] bit values.
#include "subu.hpp"
int main () {
  do_test<15> ();
  do_test<16> ();
  do_test<17> ();
}
