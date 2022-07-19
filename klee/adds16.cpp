// Tests signed add for 15-17 bit values.
#include "adds.hpp"
int main () {
  do_test<15> ();
  do_test<16> ();
  do_test<17> ();
}
