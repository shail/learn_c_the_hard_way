#ifndef _ex22_h
#define _ex22_h

// makes THE_SIZE in ex22.c available to other .c files: extern tells the compiler "the variable exists, but
// it is in another 'external' location. This means that one .c file is going to see a variable that's been
// defined in another .c file. In this case, we're saying ex22.c has a variable THE_SIZE that will be accessed
// from ex22_main.c
extern int THE_SIZE;

// gets and sets an internal static varible in ex22.c
int get_age();
void set_age(int age);

// updates a static variable that's inside update_ratio
double update_ratio(double ratio);

void print_size();

#endif
