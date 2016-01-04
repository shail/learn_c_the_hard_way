#include "ex22.h"
#include "dbg.h"
// making a const which stands for constant and is an alternative to using a define to create a constant
// variable
const char *MY_NAME = "Shail Patel";

void scope_demo(int count) {
  log_info("Count is: %d", count);

  // if-statement that starts a new scope block, the version of count in here is actually a whole new variable
  if (count > 10) {
    int count = 100; // BAD! BUGS!

    log_info("Count in this scope is %d", count);
  }

  // this line prints out the parameter that was provided
  log_info("Count is at exit: %d", count);

  count = 3000;

  log_info("Count after assign: %d", count);
}

int main(int argc, char *argv[]) {
  // test out THE_AGE accessors
  log_info("My name: %s, age: %d", MY_NAME, get_age());

  // uses set_age in ex22.c to change THE_AGE and then print it out
  set_age(100);

  log_info("My age is now: %d", get_age());

  // test out THE_SIZE extern & accessing it directly, and demonstrating that it's actually changing in that
  // file
  log_info("THE SIZE is %d", THE_SIZE);
  print_size();

  THE_SIZE = 9;

  log_info("THE_SIZE is now: %d", THE_SIZE);
  print_size();

  // test the ratio function static
  // show how the static variable ratio inside update_ratio is maintained between function calls
  log_info("Ratio at first: %f", update_ratio(2.0));
  log_info("Ratio again: %f", update_ratio(10.0));
  log_info("Ratio once more: %f", update_ratio(300.0));

  // test the scope demo
  // local count variable remains unchanged, passing in a variable like this will not let you change it in the
  // function, to do that you need to use a pointer
  int count = 4;
  scope_demo(count);
  scope_demo(count * 20);

  log_info("count after calling scope_demo: %d", count);

  return 0;
}

/* In the strictest sense, everything in C is pass-by-value. Pass-by-value means we are passing a copy of the
 * variable to a function. C can pass a pointer but that is still pass-by-value because it is copying the
 * value of the pointer, the address into the function. When passing a pointer in C, the syntax requires a
 * dereference to be applied to get the value. Passing a reference is when you pass a pointer.
 */
