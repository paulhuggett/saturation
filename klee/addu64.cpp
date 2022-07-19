// Test unsigned add for 63 and 64 bit values.
#include "addu.hpp"
int main () {
  do_test<63> ();
  do_test<64> ();
}
