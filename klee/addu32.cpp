// Test unsigned add for 24 and 31-33 bit values.
#include "addu.hpp"
int main () {
  do_test<24> ();
  do_test<31> ();
  do_test<32> ();
  do_test<33> ();
}
