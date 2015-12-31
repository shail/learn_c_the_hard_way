/* This exercise will require 3 files:
 * ex22.h -- a header file that sets up some external variables and some functions
 * ex22.c -- not your main like normal, but instead a source file that will become a object file ex22.0 which
 * will have some functions and varibles in it defined from ex22.h
 * ex22_main.c -- the actual main that will include the other two and demonstrate what they contain as well as
 * other scope concepts
 */

#include <stdio.h>
#include "ex22.h"
#include "dbg.h"

// this is the variable you declared extern that you'll play with from ex22_main.c
int THE_SIZE = 1000;

// static here is kind of the opposite of extern, and says that the variable is only used in this .c file and
// shouldn't be available to other parts of the program. static at the file level is different than in other
// places
static int THE_AGE = 37;

// these two functions take the static variable THE_AGE and expose it to other parts of the program, but you
// can't access THE_AGE directly
int get_age() {
  return THE_AGE;
}

void set_age(int age) {
  THE_AGE = age;
}

// this takes a new ratio value and returns the old one, it uses a function level static variable ratio to
// keep track of what the ratio currently is
double update_ratio(double new_ratio) {
  // if you declare a variable in a function static, then that variable acts like a static defined in the
  // file, but it is only accessible from that function, it is a way of creating constant state for a
  // function, but in reality it's rarely used in modern C programming because they are hard to use with
  // threads
  static double ratio = 1.0;

  double old_ratio = ratio;
  ratio = new_ratio;

  return old_ratio;
}

// Prints out what ex22.c thinks THE_SIZE is currently
void print_size() {
  log_info("I think size is %d", THE_SIZE);
}
