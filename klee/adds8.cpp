// Tests signed add for 7-9 bit values.
#include "adds.hpp"
int main () {
  do_test<7> ();
  do_test<8> ();
  do_test<9> ();
}
